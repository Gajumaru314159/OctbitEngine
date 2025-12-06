//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#pragma once

namespace ob::rhi {

    //! @brief  ユーティリティ
    class Utility {
    public:

        //! @brief  デバッグレイヤメッセージを取得
        static String GetDebugLayerLastString(ID3D12Device* pDevice,s32 count=3);

        //! @brief  エラーメッセージを取得
        static String GetErrorMessage(DWORD errorCode);


        //! @brief  HRESULTのエラーログを出力
        static void OutputErrorLog(HRESULT result, StringView message = "");


        //! @brief  HRESULTのFatalログを出力
        static void OutputFatalLog(HRESULT result, StringView message = "");

        
        //! @brief  D3D12_RESOURCE_DESC から D3D12_SRV_DIMENSIONを取得
        static D3D12_SRV_DIMENSION GetSrvDimension(const D3D12_RESOURCE_DESC& desc);


        //! @brief  ID3D12Objectに名前を設定
        static void SetName(ID3D12Object* pObject,StringView name);

    };

}