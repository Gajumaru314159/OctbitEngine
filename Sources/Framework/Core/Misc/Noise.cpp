//***********************************************************
//! @file
//! @brief		ノイズ
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/Misc/Noise.h>
#include <Framework/Core/Math/Math.h>


namespace ob::core {

    namespace {

        const u8 s_perm[] = {
            151,160,137,91,90,15,
            131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
            190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
            88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
            77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
            102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
            135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
            5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
            223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
            129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
            251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
            49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
            138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180
        };

        inline constexpr f32 fade(const f32 t) noexcept {
            return t * t * t * (t * (t * 6.0f - 15.0f) + 10.0f);
        }

        inline constexpr f32 grad(const u8 hash, const f32 x, const f32 y, const f32 z) noexcept {
            const u8 h = hash & 15;
            const f32 u = h < 8 ? x : y;
            const f32 v = h < 4 ? y : h == 12 || h == 14 ? x : z;
            return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
        }

    }

    //@―---------------------------------------------------------------------------
    //! @brief  パーリンノイズ
    //@―---------------------------------------------------------------------------
    f32 Noise::Perlin(f32 x, f32 y, f32 z) {

        const f32 _x = Math::Floor(x);
        const f32 _y = Math::Floor(y);
        const f32 _z = Math::Floor(z);

        const s32 ix = static_cast<s32>(_x) & 255;
        const s32 iy = static_cast<s32>(_y) & 255;
        const s32 iz = static_cast<s32>(_z) & 255;

        const f32 fx = (x - _x);
        const f32 fy = (y - _y);
        const f32 fz = (z - _z);

        const f32 u = fade(fx);
        const f32 v = fade(fy);
        const f32 w = fade(fz);

        const std::uint8_t A = (s_perm[ix & 255] + iy) & 255;
        const std::uint8_t B = (s_perm[(ix + 1) & 255] + iy) & 255;

        const std::uint8_t AA = (s_perm[A] + iz) & 255;
        const std::uint8_t AB = (s_perm[(A + 1) & 255] + iz) & 255;

        const std::uint8_t BA = (s_perm[B] + iz) & 255;
        const std::uint8_t BB = (s_perm[(B + 1) & 255] + iz) & 255;

        const f32 p0 = grad(s_perm[AA], fx, fy, fz);
        const f32 p1 = grad(s_perm[BA], fx - 1, fy, fz);
        const f32 p2 = grad(s_perm[AB], fx, fy - 1, fz);
        const f32 p3 = grad(s_perm[BB], fx - 1, fy - 1, fz);
        const f32 p4 = grad(s_perm[(AA + 1) & 255], fx, fy, fz - 1);
        const f32 p5 = grad(s_perm[(BA + 1) & 255], fx - 1, fy, fz - 1);
        const f32 p6 = grad(s_perm[(AB + 1) & 255], fx, fy - 1, fz - 1);
        const f32 p7 = grad(s_perm[(BB + 1) & 255], fx - 1, fy - 1, fz - 1);

        const f32 q0 = Math::Lerp(p0, p1, u);
        const f32 q1 = Math::Lerp(p2, p3, u);
        const f32 q2 = Math::Lerp(p4, p5, u);
        const f32 q3 = Math::Lerp(p6, p7, u);

        const f32 r0 = Math::Lerp(q0, q1, v);
        const f32 r1 = Math::Lerp(q2, q3, v);

        return Math::Lerp(r0, r1, w);
    }

    //@―---------------------------------------------------------------------------
    //! @brief  パーリンノイズ
    //! 
    //! @param octaves     何回重ね合わせるか
    //! @param persistence 詳細なノイズの強度
    //@―---------------------------------------------------------------------------
    f32 Noise::PerlinOctave(s32 octaves, f32 persistence, f32 x, f32 y, f32 z) {
        f32 result = 0.0f;
        f32 amplitude = 1.0f;

        for (s32 i = 0; i < octaves; ++i) {
            result += (Perlin(x, y, z) * amplitude);
            x *= 2.0f;
            y *= 2.0f;
            z *= 2.0f;
            amplitude *= persistence;
        }

        return result;
    }

}// namespace ob