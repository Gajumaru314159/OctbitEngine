//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include "ShaderImpl.h"
#include <d3dcompiler.h>
#include <Framework/Core/String/StringEncoder.h>
#include <Framework/Core/Misc/Blob.h>
#include <Framework/RHI/Shader.h>
#include <Plugins/DirectX12RHI/DirectX12RHI.h>
#include <Plugins/DirectX12RHI/Utility/Utility.h>

namespace ob::rhi::dx12 {

    const wchar_t* getEntryW(ShaderStage stage) {
        switch (stage)
        {
        case ShaderStage::Vertex:          return L"VS_Main";
        case ShaderStage::Hull:            return L"HS_Main";
        case ShaderStage::Domain:          return L"DS_Main";
        case ShaderStage::Geometry:        return L"GS_Main";
        case ShaderStage::Pixel:           return L"PS_Main";
        case ShaderStage::Compute:         return L"CS_Main";
        default:                            return L"";
        }
    }
    const wchar_t* getShadingModelW(ShaderStage stage) {
        switch (stage)
        {
        case ShaderStage::Vertex:          return L"vs_6_0";
        case ShaderStage::Hull:            return L"hs_6_0";
        case ShaderStage::Domain:          return L"ds_6_0";
        case ShaderStage::Geometry:        return L"gs_6_0";
        case ShaderStage::Pixel:           return L"ps_6_6";
        case ShaderStage::Compute:         return L"cs_6_0";
        default:                            return L"";
        }
    }



    //@―---------------------------------------------------------------------------
    //! @brief				シェーダーコードからシェーダーオブジェクトを生成
    //!
    //! @param src			シェーダコード
    //! @param stage		シェーダステージ
    //! @param errorDest	エラー出力先文字列
    //@―---------------------------------------------------------------------------
    ShaderImpl::ShaderImpl(DirectX12RHI& device,const String& code, ShaderStage stage, StringView name)
        : m_name(name)
    {
        compile(device, code, stage);
    }



    //@―---------------------------------------------------------------------------
    //! @brief				バイナリからシェーダーオブジェクトを生成
    //!
    //! @param src			バイナリ
    //! @param stage		シェーダステージ
    //! @param errorDest	エラー出力先文字列
    //@―---------------------------------------------------------------------------
    ShaderImpl::ShaderImpl(BlobView blob, ShaderStage stage, StringView name)
        : m_name(name)
    {
        // チャンクタイプをチェック
        bool isShaderBlob = (!blob.empty()) && *reinterpret_cast<const u32*>(blob.data()) == 0x43425844;
        if (!isShaderBlob) {
            LOG_ERROR_EX("Graphic", "シェーダではないバイナリファイルから構築しようとしました。");
        }
        m_shaderBlob = Blob(blob.data(),blob.size());
    }


    //@―---------------------------------------------------------------------------
    //! @brief              妥当な状態か
    //@―---------------------------------------------------------------------------
    bool ShaderImpl::isValid()const {
        if (!m_shaderBlob.empty())return true;
        if (m_shaderBolb2)return true;
        return false;
    }

    
    //@―---------------------------------------------------------------------------
    //! @brief      名前を取得
    //@―---------------------------------------------------------------------------
    const String& ShaderImpl::getName()const {
        return m_name;
    }


    //@―---------------------------------------------------------------------------
    //! @brief  シェーダステージを取得
    //@―---------------------------------------------------------------------------
    ShaderStage ShaderImpl::getStage()const {
        return m_stage;
    }


    //@―---------------------------------------------------------------------------
    //! @brief  バイナリデータを取得
    //@―---------------------------------------------------------------------------
    const void* ShaderImpl::getBinaryData()const {
        if (!m_shaderBlob.empty())return m_shaderBlob.data();
        if (m_shaderBolb2)return m_shaderBolb2->GetBufferPointer();
        return nullptr;
    }


    //@―---------------------------------------------------------------------------
    //! @brief  バイナリサイズを取得
    //@―---------------------------------------------------------------------------
    SIZE_T ShaderImpl::getBinarySize()const {
        if (!m_shaderBlob.empty())return m_shaderBlob.size();
        if (m_shaderBolb2)return m_shaderBolb2->GetBufferSize();
        return 0;
    }


    //@―---------------------------------------------------------------------------
    //! @brief				初期化
    //@―---------------------------------------------------------------------------
    void ShaderImpl::compile(DirectX12RHI& device, StringView code, ShaderStage stage) {

        HRESULT result;

        // シェーダーコード
        DxcBuffer buffer;
        buffer.Ptr = code.data();
        buffer.Size = code.size();
        buffer.Encoding = 0;

        // コンパイル引数
        const wchar_t* args[] = {
            L"-E",
            getEntryW(stage),
            L"-T",
            getShadingModelW(stage)
        };

        // コンパイル
        ComPtr<IDxcResult> resultBlob;
        result = device.getShaderCompiler()->Compile(
            &buffer,
            args,
            std::size(args),
            NULL,
            IID_PPV_ARGS(&resultBlob)
        );
        if (FAILED(result)) {
            Utility::OutputErrorLog(result, "シェーダコンパイルエラー");
            return;
        }

        // エラーチェック
        ComPtr<IDxcBlobUtf8> errors{};
        ComPtr<IDxcBlobUtf16> outputName{};
        result = resultBlob->GetOutput(DXC_OUT_ERRORS,IID_PPV_ARGS(&errors),&outputName);
        if (FAILED(result)) {
            Utility::OutputErrorLog(result, "シェーダコンパイルエラー");
            return;
        }
        if (errors->GetBufferSize() != 0) {
            LOG_ERROR_EX("Graphic", "{}", StringView(errors->GetStringPointer(), errors->GetStringLength()));
            return;
        }

        // バイナリ取得
        result = resultBlob->GetResult(m_shaderBolb2.ReleaseAndGetAddressOf());
        if (FAILED(result)) {
            Utility::OutputErrorLog(result, "シェーダコンパイルエラー");
            return;
        }

    }

}