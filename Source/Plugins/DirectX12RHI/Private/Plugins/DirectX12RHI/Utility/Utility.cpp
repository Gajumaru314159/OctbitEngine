//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#include "Utility.h"
#include <Framework/Core/String/StringEncoder.h>
#include <Framework/Core/Misc/Blob.h>

namespace ob::rhi {

    //! @brief  エラーメッセージを取得
    String Utility::GetErrorMessage(DWORD errorCode) {
        WCHAR buffer[256];
        DWORD dwFlags = FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS;
        auto result = ::FormatMessageW(
            dwFlags,
            nullptr,
            errorCode,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            buffer,
            (DWORD)std::size(buffer),
            nullptr
        );

        if (0 == result) {
            String message;
            StringEncoder::Encode(buffer, message);
            return "不明なエラー";
        }
        String message;
        StringEncoder::Encode(buffer, message);
        if (2 <= message.size())message.resize(message.size() - 2);
        String error = Format("{0}", message.c_str());

        return std::move(error);
    }

    //! @brief  デバッグレイヤメッセージを取得
    String Utility::GetDebugLayerLastString(ID3D12Device* pDevice, s32 count) {

        String message;

#ifdef OB_DEBUG

        String desc;
        Blob blob;

        ComPtr<ID3D12InfoQueue> infoQueue;
        if (pDevice&&SUCCEEDED(pDevice->QueryInterface(IID_PPV_ARGS(infoQueue.ReleaseAndGetAddressOf())))) {
            u64 storedCount = infoQueue->GetNumStoredMessages();
            u64 swapChainCount = std::clamp<u64>(storedCount, 0,count);
            for (u64 i = 0; i < swapChainCount; ++i) {
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

                message = "DebugLayer: ";
                switch (pMessage->Severity)
                {
                case D3D12_MESSAGE_SEVERITY_CORRUPTION: message += "[CORRUPTION]"; break;
                case D3D12_MESSAGE_SEVERITY_ERROR:      message += "[ERROR]"; break;
                case D3D12_MESSAGE_SEVERITY_WARNING:    message += "[WARNING]"; break;
                }
                switch (pMessage->Category)
                {
                case D3D12_MESSAGE_CATEGORY_APPLICATION_DEFINED:    message += "[APPLICATION_DEFINED]"; break;
                case D3D12_MESSAGE_CATEGORY_MISCELLANEOUS:          message += "[MISCELLANEOUS]"; break;
                case D3D12_MESSAGE_CATEGORY_INITIALIZATION:         message += "[INITIALIZATION]"; break;
                case D3D12_MESSAGE_CATEGORY_CLEANUP:                message += "[CLEANUP]"; break;
                case D3D12_MESSAGE_CATEGORY_COMPILATION:            message += "[COMPILATION]"; break;
                case D3D12_MESSAGE_CATEGORY_STATE_CREATION:         message += "[STATE_CREATION]"; break;
                case D3D12_MESSAGE_CATEGORY_STATE_SETTING:          message += "[STATE_SETTING]"; break;
                case D3D12_MESSAGE_CATEGORY_STATE_GETTING:          message += "[STATE_GETTING]"; break;
                case D3D12_MESSAGE_CATEGORY_RESOURCE_MANIPULATION:  message += "[RESOURCE_MANIPULATION]"; break;
                case D3D12_MESSAGE_CATEGORY_EXECUTION:              message += "[EXECUTION]"; break;
                case D3D12_MESSAGE_CATEGORY_SHADER:                 message += "[SHADER]"; break;
                default:                                            message += "[UNKNOWN]"; break;
                }

                
                StringEncoder::Encode(pMessage->pDescription, desc);
                message += desc;
                message += "\n";
            }
            infoQueue->ClearStoredMessages();
            infoQueue->Release();
        }
#endif

        return std::move(message);
    }

    //! @brief  HRESULTのエラーログを出力
    void Utility::OutputErrorLog(HRESULT result, StringView message) {
        LOG_ERROR_EX("Graphic", "{0}[{1:#X}:{2}]", message, result, Utility::GetErrorMessage(result));
    }


    //! @brief  HRESULTのFatalログを出力
    void Utility::OutputFatalLog(HRESULT result, StringView message) {
        LOG_FATAL_EX("Graphic", "{0}[{1:#X}:{2}]", message, result, Utility::GetErrorMessage(result));
    }


    //! @brief  D3D12_RESOURCE_DESC から D3D12_SRV_DIMENSIONを取得
    D3D12_SRV_DIMENSION Utility::GetSrvDimension(const D3D12_RESOURCE_DESC& desc) {
        switch (desc.Dimension) {
        case D3D12_RESOURCE_DIMENSION_TEXTURE1D:
            if (desc.DepthOrArraySize == 0) {
                return D3D12_SRV_DIMENSION_TEXTURE1D;
            } else {
                return D3D12_SRV_DIMENSION_TEXTURE1DARRAY;
            }
        case D3D12_RESOURCE_DIMENSION_TEXTURE2D:
            if (desc.DepthOrArraySize == 0) {
                return D3D12_SRV_DIMENSION_TEXTURE2D;
            } else {
                return D3D12_SRV_DIMENSION_TEXTURE2DARRAY;
            }
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


    //! @brief  ID3D12Objectに名前を設定
    void Utility::SetName(ID3D12Object* pObject, StringView name) {
#ifdef  OB_DEBUG
        if (pObject == nullptr)return;
        WString wname;
        StringEncoder::Encode(name, wname);
        pObject->SetName(wname.c_str());
#endif //  OB_DEBUG
    }

}