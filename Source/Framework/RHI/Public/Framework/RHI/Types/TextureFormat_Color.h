//***********************************************************
//! @file
//! @brief		カラーテクスチャフォーマット
//! @author		Gajumaru
//***********************************************************

RGBA32,     //!< RGBA(32bit)
RGBA16,     //!< RGBA(16bit)
RGBA8,      //!< RGBA(8bit)
RGB32,      //!< RGB(32bit)
RGB8,       //!< RGB(8bit)
RG32,       //!< RG(32bit)
RG16,       //!< RG(16bit)
RG8,        //!< RG(8bit)
R32,        //!< R(32bit)
R16,        //!< R(16bit)
R8,         //!< R(8bit)

R10G10B10A2,//!< HDR	※削除予定

BC1,        //!< 2値アルファ(DXT1)               @n 圧縮率1/6
BC2,        //!< 16階調アルファ(DXT2/DXT3)       @n 圧縮率1/4
BC3,        //!< 多階調アルファ(DXT4/DXT5)       @n 圧縮率1/4
BC4,        //!< 1成分(ATI1N)                    @n 圧縮率1/2
BC5,        //!< 2成分(ATI2N)                    @n 圧縮率1/2
BC6H,       //!< HDR                             @n 圧縮率1/6
BC7,        //!< BPTC                            @n 圧縮率1/4

BC1_SRGB,   //!< 2値アルファ(DXT1)               @n 圧縮率1/6
BC2_SRGB,   //!< 16階調アルファ(DXT2/DXT3)       @n 圧縮率1/4
BC3_SRGB,   //!< 多階調アルファ(DXT4/DXT5)       @n 圧縮率1/4
BC7_SRGB,   //!< BPTC                            @n 圧縮率1/4

// ETC / EAC / ASTC / PVRTC などは未対応