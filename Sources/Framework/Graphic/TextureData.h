//***********************************************************
//! @file
//! @brief		テクスチャデータ
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Misc/BlobView.h>
#include <Framework/Graphic/Types/TextureFormat.h>
#include <Framework/Core/File/Path.h>

namespace ob::graphic {

    //@―---------------------------------------------------------------------------
    //! @brief  説明
    //@―---------------------------------------------------------------------------
    class TextureData {
    public:

        //===============================================================
        // コンストラクタ / デストラクタ
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief  説明
        //@―---------------------------------------------------------------------------

        TextureData(const Path& path);
        TextureData(const void* data,size_t size);

        Size        size;
        Blob        data;

    };

    struct TextureMipsData {
        vector<TextureData> mips;
        TextureFormat format;
        Size size;
    };






    //===============================================================
    // インライン関数
    //===============================================================
    //! @cond



    //! @endcond
}// namespcae ob