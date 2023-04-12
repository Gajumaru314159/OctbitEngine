//***********************************************************
//! @file
//! @brief		テクスチャデータ
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Misc/BlobView.h>
#include <Framework/RHI/Types/TextureFormat.h>
#include <Framework/RHI/Types/TextureDesc.h>
#include <Framework/Core/File/Path.h>

namespace ob::rhi {

    //@―---------------------------------------------------------------------------
    //! @brief  説明
    //@―---------------------------------------------------------------------------
    class TextureData {
    public:

        //@―---------------------------------------------------------------------------
        //! @brief  説明
        //@―---------------------------------------------------------------------------

        TextureData(const Path& path);
        //TextureData(const void* data,size_t size);

    private:
        TextureFormat   m_format;
        TextureType     m_type;

        s32     m_arrayNum;
        Size    m_size;
        Blob    m_blob;

        //Array<TextureMipsData> m_mips;
    };

    struct TextureMipsData {
        void* data;
        size_t rowPitch;
        size_t slicePitch;
    };






    //===============================================================
    // インライン関数
    //===============================================================
    //! @cond



    //! @endcond
}