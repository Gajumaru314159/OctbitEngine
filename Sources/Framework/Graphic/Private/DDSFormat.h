//***********************************************************
//! @file
//! @brief		DDSフォーマット
//! @author		Gajumaru
//***********************************************************
#pragma once
// 参考
// https://dench.flatlib.jp/ddsformat

namespace ob::graphic {

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
        u32                 miscGlags2;         //!< DDS_MISC_FLAGS2
    };

}// namespcae ob