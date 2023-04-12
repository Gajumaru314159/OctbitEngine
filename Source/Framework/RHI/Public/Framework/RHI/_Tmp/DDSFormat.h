//***********************************************************
//! @file
//! @brief		DDSフォーマット
//! @author		Gajumaru
//***********************************************************
#pragma once
// 参考
// https://dench.flatlib.jp/ddsformat

namespace ob::rhi {

    //@―---------------------------------------------------------------------------
    //! @brief  DDSピクセルフォーマット
    //@―---------------------------------------------------------------------------
    enum DDPF:u32 {
        DDPF_ALPHAPIXELS    = 0x00000001 ,//!< RGB 以外に alpha が含まれている。つまり dwRGBAlphaBitMask が有効。
        DDPF_ALPHA	        = 0x00000002 ,//!< pixel は Alpha 成分のみ含まれている。
        DDPF_FOURCC	        = 0x00000004 ,//!< dwFourCC が有効。
        DDPF_PALETTEINDEXED4= 0x00000008 ,//!<      Palet 16 colors(DX9 以降はたぶん使用されない)
        DDPF_PALETTEINDEXED8= 0x00000020 ,//!<      Palet 256 colors(DX10 以降は使用されない)
        DDPF_RGB	        = 0x00000040 ,//!< dwRGBBitCount / dwRBitMask / dwGBitMask / dwBBitMask / dwRGBAlphaBitMask によってフォーマットが定義されていることを示す
        DDPF_LUMINANCE	    = 0x00020000 ,//!<      1ch のデータが R G B すべてに展開される
        DDPF_BUMPDUDV	    = 0x00080000 ,//!<      pixel が符号付であることを示す(本来は bump 用) DX10以降は使用しない
    };

    //@―---------------------------------------------------------------------------
    //! @brief  DDSピクセルフォーマット
    //@―---------------------------------------------------------------------------
    enum DDSD :u32 {
        DDSD_CAPS           = 0x1,      // すべての.dds ファイルで必要です。
        DDSD_HEIGHT	        = 0x2,      // すべての.dds ファイルで必要です。	0x2
        DDSD_WIDTH	        = 0x4,      // すべての.dds ファイルで必要です。	
        DDSD_PITCH	        = 0x8,      // 非圧縮テクスチャにピッチが指定されている場合に必要です。	0x8
        DDSD_PIXELFORMAT    = 0x1000,   // すべての.dds ファイルで必要です。	0x1000
        DDSD_MIPMAPCOUNT	= 0x20000,  // mipmapped テクスチャで必要です。	0x20000
        DDSD_LINEARSIZE	    = 0x80000,  // 圧縮テクスチャにピッチが指定されている場合に必要です。
        DDSD_DEPTH	        = 0x800000, // 深度テクスチャで必要です。
    };


    //@―---------------------------------------------------------------------------
    //! @brief  DDSピクセルフォーマット
    //@―---------------------------------------------------------------------------
    enum DDSCAPS :u32 {
        DDSCAPS_ALPHA	=0x00000002,//Alpha が含まれている場合(あまり参照されない)
        DDSCAPS_COMPLEX	=0x00000008,//複数のデータが含まれている場合。Palette / Mipmap / Cubemap / VolumeTexture では On にする。
        DDSCAPS_TEXTURE	=0x00001000,//常に On
        DDSCAPS_MIPMAP	=0x00400000,//MipMap が存在する場合。(dwFlags の DDSD_MIPMAPCOUNT が On でかつ dwMipMapCount が 2以上の場合に On)
    };

    enum DDSCAPS2 :u32 {
        DDSCAPS2_CUBEMAP	        = 0x00000200,//Cubemap の場合
        DDSCAPS2_CUBEMAP_POSITIVEX	= 0x00000400,//Cubemap + X が含まれている
        DDSCAPS2_CUBEMAP_NEGATIVEX	= 0x00000800,//Cubemap - X が含まれている
        DDSCAPS2_CUBEMAP_POSITIVEY	= 0x00001000,//Cubemap + Y が含まれている
        DDSCAPS2_CUBEMAP_NEGATIVEY	= 0x00002000,//Cubemap - Y が含まれている
        DDSCAPS2_CUBEMAP_POSITIVEZ	= 0x00004000,//Cubemap + Z が含まれている
        DDSCAPS2_CUBEMAP_NEGATIVEZ	= 0x00008000,//Cubemap - Z が含まれている
        DDSCAPS2_VOLUME	            = 0x00400000,//VolumeTexture の場合
    };

    enum DDS_DIMENSION:u32 {
        DDS_DIMENSION_UNKNOWN = 0,
        DDS_DIMENSION_BUFFER = 1,
        DDS_DIMENSION_TEXTURE1D = 2,
        DDS_DIMENSION_TEXTURE2D = 3,
        DDS_DIMENSION_TEXTURE3D = 4
    };

    enum DDS_MISC_FLAG:u8 {
        DDS_MISC_FLAG_GENERATE_MIPS = 0x1L,
        DDS_MISC_FLAG_SHARED = 0x2L,
        DDS_MISC_FLAG_TEXTURECUBE = 0x4L,
        DDS_MISC_FLAG_DRAWINDIRECT_ARGS = 0x10L,
        DDS_MISC_FLAG_BUFFER_ALLOW_RAW_VIEWS = 0x20L,
        DDS_MISC_FLAG_BUFFER_STRUCTURED = 0x40L,
        DDS_MISC_FLAG_RESOURCE_CLAMP = 0x80L,
        DDS_MISC_FLAG_SHARED_KEYEDMUTEX = 0x100L,
        DDS_MISC_FLAG_GDI_COMPATIBLE = 0x200L,
        DDS_MISC_FLAG_SHARED_NTHANDLE = 0x800L,
        DDS_MISC_FLAG_RESTRICTED_CONTENT = 0x1000L,
        DDS_MISC_FLAG_RESTRICT_SHARED_RESOURCE = 0x2000L,
        DDS_MISC_FLAG_RESTRICT_SHARED_RESOURCE_DRIVER = 0x4000L,
        DDS_MISC_FLAG_GUARDED = 0x8000L,
        DDS_MISC_FLAG_TILE_POOL = 0x20000L,
        DDS_MISC_FLAG_TILED = 0x40000L,
        DDS_MISC_FLAG_HW_PROTECTED = 0x80000L,
        DDS_MISC_FLAG_SHARED_DISPLAYABLE,
        DDS_MISC_FLAG_SHARED_EXCLUSIVE_WRITER
    };

    //@―---------------------------------------------------------------------------
    //! @brief  DDSピクセルフォーマット
    //@―---------------------------------------------------------------------------
    struct DDS_PIXEL_FORMAT {
        u32     size;           //!< 構造体サイズ 32
        DDPF    flags;          //!< pixel フォーマットを表す DDPF_* の組み合わせ
        u8      fourCC[4];      //!< フォーマットが FourCC で表現される場合に使用する
        u32     RGBBitCount;    //!< 1ピクセルのビット数
        u32     RBitMask;       //!< RGBフォーマット時のRマスク
        u32     GBitMask;       //!< RGBフォーマット時のGマスク
        u32     BBitMask;       //!< RGBフォーマット時のBマスク
        u32     ABitMask;       //!< RGBフォーマット時のAマスク
    };


    //@―---------------------------------------------------------------------------
    //! @brief  DDSフォーマット
    //@―---------------------------------------------------------------------------
    struct DDS_HEADER {
        u8                  magic[4];           //!< マジックナンバー " SDD" 0x20534444
        u32                 size;               //!< 構造体サイズ 124
        u32                 flags;              //!< ヘッダ内の有効な情報 DDSD_* の組み合わせ
        u32                 height;             //!< 画像の高さ
        u32                 width;              //!< 画像の幅   y size
        u32                 pitchOrLinearSize;  //!< 横1 line の byte 数 (pitch)　or 1面分の byte 数 (linearsize)
        u32                 depth;              //!< 画像の奥行き
        u32                 mipMapCount;        //!< 含まれている mipmap レベル数
        u32                 reserved1[11];      //!< 予約済み領域
        DDS_PIXEL_FORMAT    ddspf;              //!< ピクセルフォーマット
        u32                 caps;               //!< ミップマップなどのフラグ指定用
        u32                 caps2;              //!< Cube Texture / Volume Texture などのフラグ指定用
        u32                 reservedCaps[2];    //!< 予約済み領域
        u32                 reserved2;          //!< 予約済み領域
    };


    //@―---------------------------------------------------------------------------
    //! @brief  DDS拡張フォーマット
    //@―---------------------------------------------------------------------------
    struct DDS_HEADER_DXT10 {
        u32                 dxgiFormat;         //!< DXGI_FORMAT
        u32                 resourceDimension;  //!< 
        u32                 miscFlag;           //!< D3D11_RESOURCE_MISC_FLAG
        u32                 arraySize;          //!< 
        u32                 miscFlags2;         //!< DDS_MISC_FLAGS2
    };

}