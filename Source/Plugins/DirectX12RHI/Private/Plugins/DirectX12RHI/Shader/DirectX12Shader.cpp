//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#include "DirectX12Shader.h"
#include <d3dcompiler.h>
#include <Framework/Core/String/StringEncoder.h>
#include <Framework/Core/Misc/Blob.h>
#include <Framework/RHI/Shader.h>
#include <Plugins/DirectX12RHI/DirectX12Device.h>
#include <Plugins/DirectX12RHI/Utility/Utility.h>

namespace ob::rhi {

    static const wchar_t* getEntryW(ShaderStage stage) {
        switch (stage)
        {
        case ShaderStage::Vertex:          return L"VS_Main";
        case ShaderStage::Hull:            return L"HS_Main";
        case ShaderStage::Domain:          return L"DS_Main";
        case ShaderStage::Geometry:        return L"GS_Main";
        case ShaderStage::Pixel:           return L"PS_Main";
        case ShaderStage::Compute:         return L"CS_Main";
        default:                           return L"";
        }
    }
    static const wchar_t* getShadingModelW(ShaderStage stage) {
        switch (stage)
        {
        case ShaderStage::Vertex:          return L"vs_6_6";
        case ShaderStage::Hull:            return L"hs_6_6";
        case ShaderStage::Domain:          return L"ds_6_6";
        case ShaderStage::Geometry:        return L"gs_6_6";
        case ShaderStage::Pixel:           return L"ps_6_6";
        case ShaderStage::Compute:         return L"cs_6_6";
        default:                           return L"";
        }
    }


    //! @brief				シェーダーコードからシェーダーオブジェクトを生成
    DirectX12Shader::DirectX12Shader(DirectX12Device& device, const ShaderCompileDesc& desc) 
		: m_name(desc.name)
        , m_stage(desc.stage)
    {
        compile(device, desc);
    }



    //! @brief				バイナリからシェーダーオブジェクトを生成
    //!
    //! @param src			バイナリ
    //! @param stage		シェーダステージ
    //! @param errorDest	エラー出力先文字列
    DirectX12Shader::DirectX12Shader(BlobView blob, ShaderStage stage, StringView name)
        : m_name(name)
        , m_stage(stage)
    {
        // チャンクタイプをチェック
        bool isShaderBlob = (!blob.empty()) && *reinterpret_cast<const u32*>(blob.data()) == 0x43425844;
        if (!isShaderBlob) {
            LOG_ERROR_EX("Graphic", "シェーダではないバイナリファイルから構築しようとしました。");
        }
        m_shaderBlob = Blob(blob.data(),blob.size());

        manage();
    }


    //! @brief              妥当な状態か
    bool DirectX12Shader::isValid()const {
        if (!m_shaderBlob.empty())return true;
        if (m_shaderBolb2)return true;
        return false;
    }

    
    //! @brief      名前を取得
    const String& DirectX12Shader::getName()const {
        return m_name;
    }


    //! @brief  シェーダステージを取得
    ShaderStage DirectX12Shader::getStage()const {
        return m_stage;
    }


    //! @brief  バイナリデータを取得
    const void* DirectX12Shader::getBinaryData()const {
        if (!m_shaderBlob.empty())return m_shaderBlob.data();
        if (m_shaderBolb2)return m_shaderBolb2->GetBufferPointer();
        return nullptr;
    }


    //! @brief  バイナリサイズを取得
    SIZE_T DirectX12Shader::getBinarySize()const {
        if (!m_shaderBlob.empty())return m_shaderBlob.size();
        if (m_shaderBolb2)return m_shaderBolb2->GetBufferSize();
        return 0;
    }

    //! @brief				初期化
    void DirectX12Shader::compile(DirectX12Device& device, const ShaderCompileDesc& desc) {

        HRESULT result;

        // シェーダーコード
        DxcBuffer buffer;
        buffer.Ptr = desc.code.data();
        buffer.Size = desc.code.size();
        buffer.Encoding = CP_UTF8;

        Vector<WString> args{
            L"-E",
            getEntryW(desc.stage),
            L"-T",
            getShadingModelW(desc.stage),
            L"-encoding",
            L"utf8",
            L"-D",
            L"D3D",
            L"/Zi",
            L"-Qembed_debug"
        };

        for (auto& macro : desc.macros) {
            args.push_back(L"-D");
            StringEncoder::Encode(macro, args.emplace_back());
        }
        for (auto& directory : desc.directories) {
            args.push_back(L"-I");
            StringEncoder::Encode(directory, args.emplace_back());
        }

        Vector<const wchar_t*> pargs;

		for (auto& arg : args) {
			pargs.push_back(arg.data());
		}

        auto printArgs = [&]() {
            WString wargsText;
            for (auto& arg : args) {
                wargsText += L" ";
                wargsText += arg;
            }
            String argsText;
            StringEncoder::Encode(wargsText, argsText);
            LOG_INFO("{}", argsText);
        };

        // コンパイル
        // TODO スレッド安全性の確認
        ComPtr<IDxcResult> resultBlob;
        result = device.getShaderCompiler()->Compile(
            &buffer,
            pargs.data(),
            pargs.size(),
            device.getIncludeHandler().Get(),
            IID_PPV_ARGS(&resultBlob)
        );
        if (FAILED(result)) {
            Utility::OutputErrorLog(result, "シェーダコンパイルエラー。APIの呼び出し方が間違っています。");
            printArgs();
            return;
        }

        // エラーチェック
        ComPtr<IDxcBlobUtf8> errors{};
        ComPtr<IDxcBlobUtf16> outputName{};
        String errorMessage;
        if (SUCCEEDED(resultBlob->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&errors), &outputName))) {
            if (errors->GetBufferSize() != 0) {
                errorMessage = StringView(errors->GetStringPointer(), errors->GetStringLength());
            }
        }

        HRESULT blobStatus;
        if (FAILED(resultBlob->GetStatus(&blobStatus)) || FAILED(blobStatus))
        {
            LOG_ERROR_EX("Graphic", "シェーダコンパイルに失敗しました\n{}", errorMessage);
            return;
        } else if (!errorMessage.empty()) {
            LOG_WARNING_EX("Graphic", "シェーダコンパイル時に警告が発生しています\n{}", errorMessage);
        }

        // バイナリ取得
        result = resultBlob->GetResult(m_shaderBolb2.ReleaseAndGetAddressOf());
        if (FAILED(result)) {
            Utility::OutputErrorLog(result, "シェーダーバイナリの取得に失敗しました");
            return;
        }

    }

}