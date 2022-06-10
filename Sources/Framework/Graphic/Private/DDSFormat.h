//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once

namespace ob::graphic {

    struct DDS_PIXEL_FORMAT {
        u32     size;           //!< 構造体サイズ 32
        u32     flags;          //!< pixel フォーマットを表す DDPF_* の組み合わせ
        u32     fourCC;         //!< フォーマットが FourCC で表現される場合に使用する
        u32     RGBBitCount;    //!< 1ピクセルのビット数
        u32     RBitMask;       //!< RGBフォーマット時のRマスク
        u32     GBitMask;       //!< RGBフォーマット時のGマスク
        u32     BBitMask;       //!< RGBフォーマット時のBマスク
        u32     ABitMask;       //!< RGBフォーマット時のAマスク
    };

    struct DDS_HEADER {

        u32                 magic;              //!< マジックナンバー " SDD" 0x20534444
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

    struct DDS_HEADER_DXT10 {
        u32                 dxgiFormat;         //!< DXGI_FORMAT
        u32                 resourceDimension;  //!< 
        u32                 miscFlag;           //!< D3D11_RESOURCE_MISC_FLAG
        u32                 arraySize;          //!< 
        u32                 miscGlags2;         //!< DDS_MISC_FLAGS2
    };





    //===============================================================
    // インライン関数
    //===============================================================
    //! @cond



    //! @endcond
}// namespcae ob