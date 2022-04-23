//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include "Utility.h"
#include <Framework/Foundation/String/StringEncoder.h>

namespace ob::graphic::dx12 {

    //@―---------------------------------------------------------------------------
    //! @brief  HRESULTのエラーログを出力
    //@―---------------------------------------------------------------------------
    void Utility::outputErrorLog(HRESULT result, StringView message){
        LOG_ERROR_EX("Graphic", "{0}[{1:#X}:{2}]", message,result, Utility::getErrorMessage(result));
    }

    //@―---------------------------------------------------------------------------
    //! @brief  エラーメッセージを取得
    //@―---------------------------------------------------------------------------
    String Utility::getErrorMessage(DWORD errorCode) {
        WCHAR buffer[256];
        DWORD dwFlags = FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS;
        auto result = ::FormatMessageW(
            dwFlags,
            nullptr,
            errorCode,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            buffer,
            sizeof(buffer),
            nullptr
        );

        if (0 == result) {
            String message;
            StringEncoder::Encode(buffer, message);
            return TC("不明なエラー");
        }
        String message;
        StringEncoder::Encode(buffer, message);
        if (2 <= message.size())message.resize(message.size() - 2);
        String error = fmt::format(TC("{0}"), message.c_str());

        return move(error);
    }


    //@―---------------------------------------------------------------------------
    //! @brief  エラーメッセージを取得
    //@―---------------------------------------------------------------------------
    D3D12_SRV_DIMENSION Utility::getSrvDimention(const D3D12_RESOURCE_DESC& desc) {
        switch (desc.Dimension) {
        case D3D12_RESOURCE_DIMENSION_TEXTURE1D:
            if (desc.DepthOrArraySize == 0) {
                return D3D12_SRV_DIMENSION_TEXTURE1D;
            } else {
                return D3D12_SRV_DIMENSION_TEXTURE1DARRAY;
            }
            break;

        case D3D12_RESOURCE_DIMENSION_TEXTURE2D:
            if (desc.DepthOrArraySize == 0) {
                return D3D12_SRV_DIMENSION_TEXTURE2D;
            } else {
                return D3D12_SRV_DIMENSION_TEXTURE2DARRAY;
            }
            break;

        case D3D12_RESOURCE_DIMENSION_TEXTURE3D:
            return D3D12_SRV_DIMENSION_TEXTURE3D;

        case D3D12_RESOURCE_DIMENSION_BUFFER:
            return D3D12_SRV_DIMENSION_BUFFER;
        }
        return D3D12_SRV_DIMENSION_UNKNOWN;

        // D3D12_SRV_DIMENSION_TEXTURE2DMS
        // D3D12_SRV_DIMENSION_TEXTURE2DMSARRAY
        // D3D12_SRV_DIMENSION_TEXTURECUBE
        // D3D12_SRV_DIMENSION_TEXTURECUBEARRAY
        // D3D12_SRV_DIMENSION_RAYTRACING_ACCELERATION_STRUCTURE
    }

}// namespace ob::graphic::dx12