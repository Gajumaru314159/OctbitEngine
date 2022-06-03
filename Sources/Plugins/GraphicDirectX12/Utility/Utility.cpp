//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include "Utility.h"
#include <Framework/Core/String/StringEncoder.h>
#include <Framework/Core/Misc/Blob.h>

namespace ob::graphic::dx12 {

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
    //! @brief  デバッグレイヤメッセージを取得
    //@―---------------------------------------------------------------------------
    String Utility::getDebugLayerLastString(ID3D12Device* pDevice, s32 count) {

        String message;

#ifdef OB_DEBUG

        String desc;
        Blob blob;

        ComPtr<ID3D12InfoQueue> infoQueue;
        if (pDevice&&SUCCEEDED(pDevice->QueryInterface(IID_PPV_ARGS(infoQueue.ReleaseAndGetAddressOf())))) {
            u64 storedCount = infoQueue->GetNumStoredMessages();
            u64 displayCount = get_min<u64>(storedCount, get_max(count,0));
            for (u64 i = 0; i < displayCount; ++i) {
                u64 index = (storedCount - 1) - i;
                SIZE_T messageLength = 0;
                if (FAILED(infoQueue->GetMessageW(index, nullptr, &messageLength))) {
                    continue;
                }
                blob.resize(messageLength);
                auto pMessage = reinterpret_cast<D3D12_MESSAGE*>(blob.data());
                if (FAILED(infoQueue->GetMessageW(index, pMessage, &messageLength))) {
                    continue;
                }

                message = TC("DebugLayer: ");
                switch (pMessage->Severity)
                {
                case D3D12_MESSAGE_SEVERITY_CORRUPTION: message += TC("[CORRUPTION]"); break;
                case D3D12_MESSAGE_SEVERITY_ERROR:      message += TC("[ERROR]"); break;
                case D3D12_MESSAGE_SEVERITY_WARNING:    message += TC("[WARNING]"); break;
                }
                switch (pMessage->Category)
                {
                case D3D12_MESSAGE_CATEGORY_APPLICATION_DEFINED:    message += TC("[APPLICATION_DEFINED]"); break;
                case D3D12_MESSAGE_CATEGORY_MISCELLANEOUS:          message += TC("[MISCELLANEOUS]"); break;
                case D3D12_MESSAGE_CATEGORY_INITIALIZATION:         message += TC("[INITIALIZATION]"); break;
                case D3D12_MESSAGE_CATEGORY_CLEANUP:                message += TC("[CLEANUP]"); break;
                case D3D12_MESSAGE_CATEGORY_COMPILATION:            message += TC("[COMPILATION]"); break;
                case D3D12_MESSAGE_CATEGORY_STATE_CREATION:         message += TC("[STATE_CREATION]"); break;
                case D3D12_MESSAGE_CATEGORY_STATE_SETTING:          message += TC("[STATE_SETTING]"); break;
                case D3D12_MESSAGE_CATEGORY_STATE_GETTING:          message += TC("[STATE_GETTING]"); break;
                case D3D12_MESSAGE_CATEGORY_RESOURCE_MANIPULATION:  message += TC("[RESOURCE_MANIPULATION]"); break;
                case D3D12_MESSAGE_CATEGORY_EXECUTION:              message += TC("[EXECUTION]"); break;
                case D3D12_MESSAGE_CATEGORY_SHADER:                 message += TC("[SHADER]"); break;
                default:                                            message += TC("[UNKNOWN]"); break;
                }

                
                StringEncoder::Encode(pMessage->pDescription, desc);
                message += desc;
                message += TC("\n");
            }
            infoQueue->ClearStoredMessages();
            infoQueue->Release();
        }
#endif

        return move(message);
    }

    //@―---------------------------------------------------------------------------
    //! @brief  HRESULTのエラーログを出力
    //@―---------------------------------------------------------------------------
    void Utility::outputErrorLog(HRESULT result, StringView message) {
        LOG_ERROR_EX("Graphic", "{0}[{1:#X}:{2}]", message, result, Utility::getErrorMessage(result));
    }


    //@―---------------------------------------------------------------------------
    //! @brief  HRESULTのFatalログを出力
    //@―---------------------------------------------------------------------------
    void Utility::outputFatalLog(HRESULT result, StringView message) {
        LOG_FATAL_EX("Graphic", "{0}[{1:#X}:{2}]", message, result, Utility::getErrorMessage(result));
    }


    //@―---------------------------------------------------------------------------
    //! @brief  D3D12_RESOURCE_DESC から D3D12_SRV_DIMENSIONを取得
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