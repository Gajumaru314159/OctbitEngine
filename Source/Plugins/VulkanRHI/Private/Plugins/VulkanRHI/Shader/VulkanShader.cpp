//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include <Plugins/VulkanRHI/Shader/VulkanShader.h>
#include <Framework/Core/String/StringEncoder.h>
#include <Framework/Core/Misc/Blob.h>
#include <Framework/RHI/Shader.h>
#include <Plugins/VulkanRHI/VulkanRHI.h>
#include <Plugins/VulkanRHI/Utility/Utility.h>
#include <Framework/Core/Misc/ErrorCode.h>

#include <d3dcompiler.h>
//#include <d3d12shader.h>
#include <dxcapi.h>
#include <wrl/client.h>

namespace ob::rhi::vulkan {

    const wchar_t* getEntryW(ShaderStage stage) {
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
    const wchar_t* getShadingModelW(ShaderStage stage) {
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
    VulkanShader::VulkanShader(VulkanRHI& rhi, const ShaderCompileDesc& desc)
        : m_name(desc.name)
        , m_stage(desc.stage)
    {
        compile(rhi, desc);
    }



    //! @brief				バイナリからシェーダーオブジェクトを生成
    //!
    //! @param src			バイナリ
    //! @param stage		シェーダステージ
    //! @param errorDest	エラー出力先文字列
    VulkanShader::VulkanShader(VulkanRHI& rhi,BlobView blob, ShaderStage stage, StringView name)
        : m_name(name)
        , m_stage(stage)
    {
        // チャンクタイプをチェック
        bool isShaderBlob = (!blob.empty()) && *reinterpret_cast<const u32*>(blob.data()) == 0x43425844;
        if (!isShaderBlob) {
            throw Exception("シェーダではないバイナリファイルから構築しようとしました。");
        }
        m_shaderBlob = Blob(blob.data(), blob.size());
    }


    //! @brief      名前を取得
    const String& VulkanShader::getName()const {
        return m_name;
    }


    //! @brief  シェーダステージを取得
    ShaderStage VulkanShader::getStage()const {
        return m_stage;
    }


    //! @brief				初期化
    void VulkanShader::compile(VulkanRHI& rhi, const ShaderCompileDesc& desc) {

		using namespace Microsoft::WRL;

        HRESULT result;

        // シェーダーコード
        DxcBuffer buffer;
        buffer.Ptr = desc.code.data();
        buffer.Size = desc.code.size();
        buffer.Encoding = 0;

        Vector<WString> args{
            L"-spirv",
            // L"-fspv-extension=SPV_EXT_descriptor_indexing",
            L"-E",
            getEntryW(desc.stage),
            L"-T",
            getShadingModelW(desc.stage),
            L"-encoding",
            L"utf8",
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
        result = rhi.getShaderCompiler()->Compile(
            &buffer,
            pargs.data(),
            pargs.size(),
            rhi.getIncludeHandler().Get(),
            IID_PPV_ARGS(&resultBlob)
        );
        if (FAILED(result)) {
            printArgs();
            throw Exception("shaderのコンパイルに失敗しました");
        }

        // エラーチェック
        ComPtr<IDxcBlobUtf8> errors{};
        ComPtr<IDxcBlobUtf16> outputName{};
        result = resultBlob->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&errors), &outputName);
        if (FAILED(result)) {
            printArgs();
            throw Exception("shaderのコンパイルに失敗しました");
        }
        if (errors->GetBufferSize() != 0) {
            printArgs();
            throw Exception(Format("shaderのコンパイルに失敗しました\n{}", StringView(errors->GetStringPointer(), errors->GetStringLength())));
        }

        // バイナリ取得
        ComPtr<IDxcBlob> shaderBlob;
        result = resultBlob->GetResult(shaderBlob.ReleaseAndGetAddressOf());
        if (FAILED(result)) {
            printArgs();
            throw Exception("shaderのコンパイルに失敗しました");
        }

        vk::ShaderModuleCreateInfo info;
        info.codeSize = shaderBlob->GetBufferSize();
		info.pCode = (uint32_t*)shaderBlob->GetBufferPointer();

        m_shaderModule = rhi.getDevice().createShaderModule(info, rhi.getAllocationCallbacks());

    }

}