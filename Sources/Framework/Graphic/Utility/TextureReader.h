//***********************************************************
//! @file
//! @brief		テクスチャ読み込み
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Misc/Blob.h>

namespace ob::graphic {

    //@―---------------------------------------------------------------------------
    //! @brief  テクスチャ拡張子テクスチャ読み込み
    //@―---------------------------------------------------------------------------
    enum class TextureExtension {
        Unknown,    //!< 不明な形式
        DDS,        //!< DirectDraw Surface
        HDR,        //!< High Dynamic Range Image
        TGA,        //!< Truevision Advanced Raster Graphics Adapter
        JPEG,       //!< Joint Photographic Experts Group
        TIFF,       //!< Tagged Image File Format
        GIF,        //!< Graphics Interchange Format
        PNG,        //!< Portable Network Graphics
        BMP,        //!< Bitmap
    };

    //@―---------------------------------------------------------------------------
    //! @brief  テクスチャ読み込みユーティリティ
    //@―---------------------------------------------------------------------------
    class TextureReader {
    public:

        //===============================================================
        // コンストラクタ / デストラクタ
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief  説明
        //@―---------------------------------------------------------------------------
        TextureReader(String path);
        TextureReader(String path,TextureExtension extension);


    private:



    };






    //===============================================================
    // インライン関数
    //===============================================================
    //! @cond



    //! @endcond
}// namespcae ob::graphic