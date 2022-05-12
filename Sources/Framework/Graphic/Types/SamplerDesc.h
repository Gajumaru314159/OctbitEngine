//***********************************************************
//! @file
//! @brief		サンプラー定義
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphic/Types/ShaderStage.h>

namespace ob::graphic {

#pragma region Enum

    //@―---------------------------------------------------------------------------
    //! @brief      テクスチャ・フィルター
    //@―---------------------------------------------------------------------------
    enum class TextureFillter :u32 {
        Point,  //!< 最近ピクセルのテクセルを使用
        Linear, //!< 2*2の線形補完値を使用
    };

    //@―---------------------------------------------------------------------------
    //! @brief      ミップマップ・フィルター
    //@―---------------------------------------------------------------------------
    enum class MipFillter :u32 {
        Point,  //!< 最近ピクセルのテクセルを使用
        Linear, //!< 2*2の線形補完値を使用
    };

    //@―---------------------------------------------------------------------------
    //! @brief      異方性レベル
    //@―---------------------------------------------------------------------------
    enum class Anisotropy :u32 {
        None,
        Level1,
        Level2,
        Level4,
        Level8,
        Level16,
    };

    //@―---------------------------------------------------------------------------
    //! @brief      テクスチャ繰り返し設定
    //! 
    //! @details    テクスチャサンプル時、UV座標が0～1を超えた場合にどのようにサンプルするか。
    //@―---------------------------------------------------------------------------
    enum class TextureAddress {
        Repeat, //!< 繰り返して表示
        Clamp,  //!< 端を伸ばして表示
        Mirror, //!< ミラー処理して表示
    };

#pragma endregion

    //@―---------------------------------------------------------------------------
    //! @brief      サンプラー定義
    //@―---------------------------------------------------------------------------
    struct SamplerDesc {

        TextureFillter  filter;             //!< 拡縮時のフィルタ
        MipFillter      mipFilter;          //!< ミップ時のフィルタ
        bool            zFilter;            //!< 3DテクスチャのZ軸フィルタを設定するか
        Anisotropy      anisotropy;         //!< 異方性フィルタレベル
        TextureAddress  addressU;           //!< テクスチャ・アドレス・モード U
        TextureAddress  addressV;           //!< テクスチャ・アドレス・モード V
        TextureAddress  addressW;           //!< テクスチャ・アドレス・モード W
        f32             mipLodBias;         //!< ミップLODバイアス
        Range           mipLodRange;        //!< ミップLOD範囲

    public:

        //@―---------------------------------------------------------------------------
        //! @brief      コンストラクタ
        //@―---------------------------------------------------------------------------
        SamplerDesc(
            TextureFillter filter = TextureFillter::Linear,
            TextureAddress addressU = TextureAddress::Repeat,
            TextureAddress addressV = TextureAddress::Repeat,
            TextureAddress addressW = TextureAddress::Repeat,
            f32 mipLodBias = 0,
            Anisotropy anisotropy = Anisotropy::Level16,
            f32 minLod = 0,
            f32 maxLod = (numeric_limits<f32>::max)(),
            MipFillter mipFilter = MipFillter::Linear,
            bool zFilter = false
        ) {
            setFilter(filter, mipFilter);
            setZFilter(zFilter);
            setAnisotropy(anisotropy);
            setAddress(addressU, addressV, addressW);
            setMipLod(mipLodBias, minLod, maxLod);
        }


        //@―---------------------------------------------------------------------------
        //! @brief      フィルタ設定
        //@―---------------------------------------------------------------------------
        SamplerDesc& setFilter(TextureFillter filter, MipFillter mipFilter = MipFillter::Linear) {
            this->filter = filter;
            this->mipFilter = mipFilter;
            return *this;
        }

        //@―---------------------------------------------------------------------------
        //! @brief      Zフィルタ設定
        //@―---------------------------------------------------------------------------
        SamplerDesc& setZFilter(bool zFilter) {
            this->zFilter = zFilter;
            return *this;
        }

        //@―---------------------------------------------------------------------------
        //! @brief      異方性フィルタレベル設定
        //@―---------------------------------------------------------------------------
        SamplerDesc& setAnisotropy(Anisotropy anisotropy) {
            this->anisotropy = anisotropy;
            return *this;
        }

        //@―---------------------------------------------------------------------------
        //! @brief      テクスチャ・アドレス・モード設定
        //@―---------------------------------------------------------------------------
        SamplerDesc& setAddress(TextureAddress u, TextureAddress v, TextureAddress w = TextureAddress::Repeat) {
            this->addressU = u;
            this->addressV = v;
            this->addressW = w;
            return *this;
        }

        //@―---------------------------------------------------------------------------
        //! @brief      テクスチャ・アドレス・モード設定
        //@―---------------------------------------------------------------------------
        SamplerDesc& setAddress(TextureAddress mode) {
            return setAddress(mode,mode,mode);
        }

        //@―---------------------------------------------------------------------------
        //! @brief      ミップLOD設定
        //@―---------------------------------------------------------------------------
        SamplerDesc& setMipLod(f32 bias, f32 min, f32 max) {
            this->mipLodBias = bias;
            this->mipLodRange.min = min;
            this->mipLodRange.max = max;
            return *this;
        }

    };

}// namespcae ob