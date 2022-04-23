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
        //! @brief  HRESULTのエラーログを出力
        //@―---------------------------------------------------------------------------
        static void outputErrorLog(HRESULT result, StringView message = TC(""));
        
        //@―---------------------------------------------------------------------------
        //! @brief  エラーメッセージを取得
        //@―---------------------------------------------------------------------------
        static String getErrorMessage(DWORD errorCode);

        
        static D3D12_SRV_DIMENSION getSrvDimention(const D3D12_RESOURCE_DESC& desc);

    private:



    };

}// namespcae ob::graphic::dx12