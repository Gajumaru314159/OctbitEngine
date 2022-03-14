//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphic/Types/TextureFormat.h>
#include <dxgi.h>

namespace ob::graphic::dx12 {

    //@―---------------------------------------------------------------------------
    //! @brief  説明
    //@―---------------------------------------------------------------------------
    class Utility {
    public:

        //@―---------------------------------------------------------------------------
        //! @brief  TextureFormat を DXGI_FORMAT に変換
        //@―---------------------------------------------------------------------------
        static DXGI_FORMAT convertTextureFormat(TextureFormat format);

    private:



    };

}// namespcae ob::graphic::dx12