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
#include <Plugins/VulkanRHI/Utility/Utility.h>
#include <Plugins/VulkanRHI/Device/DeviceImpl.h>
#include <Plugins/VulkanRHI/Utility/TypeConverter.h>
#include <glslang/SPIRV/GlslangToSpv.h>

namespace ob::rhi::vulkan {


	static void InitResources(TBuiltInResource* resources)
	{
		resources->maxLights = 32;
		resources->maxClipPlanes = 6;
		resources->maxTextureUnits = 32;
		resources->maxTextureCoords = 32;
		resources->maxVertexAttribs = 64;
		resources->maxVertexUniformComponents = 4096;
		resources->maxVaryingFloats = 64;
		resources->maxVertexTextureImageUnits = 32;
		resources->maxCombinedTextureImageUnits = 80;
		resources->maxTextureImageUnits = 32;
		resources->maxFragmentUniformComponents = 4096;
		resources->maxDrawBuffers = 32;
		resources->maxVertexUniformVectors = 128;
		resources->maxVaryingVectors = 8;
		resources->maxFragmentUniformVectors = 16;
		resources->maxVertexOutputVectors = 16;
		resources->maxFragmentInputVectors = 15;
		resources->minProgramTexelOffset = -8;
		resources->maxProgramTexelOffset = 7;
		resources->maxClipDistances = 8;
		resources->maxComputeWorkGroupCountX = 65535;
		resources->maxComputeWorkGroupCountY = 65535;
		resources->maxComputeWorkGroupCountZ = 65535;
		resources->maxComputeWorkGroupSizeX = 1024;
		resources->maxComputeWorkGroupSizeY = 1024;
		resources->maxComputeWorkGroupSizeZ = 64;
		resources->maxComputeUniformComponents = 1024;
		resources->maxComputeTextureImageUnits = 16;
		resources->maxComputeImageUniforms = 8;
		resources->maxComputeAtomicCounters = 8;
		resources->maxComputeAtomicCounterBuffers = 1;
		resources->maxVaryingComponents = 60;
		resources->maxVertexOutputComponents = 64;
		resources->maxGeometryInputComponents = 64;
		resources->maxGeometryOutputComponents = 128;
		resources->maxFragmentInputComponents = 128;
		resources->maxImageUnits = 8;
		resources->maxCombinedImageUnitsAndFragmentOutputs = 8;
		resources->maxCombinedShaderOutputResources = 8;
		resources->maxImageSamples = 0;
		resources->maxVertexImageUniforms = 0;
		resources->maxTessControlImageUniforms = 0;
		resources->maxTessEvaluationImageUniforms = 0;
		resources->maxGeometryImageUniforms = 0;
		resources->maxFragmentImageUniforms = 8;
		resources->maxCombinedImageUniforms = 8;
		resources->maxGeometryTextureImageUnits = 16;
		resources->maxGeometryOutputVertices = 256;
		resources->maxGeometryTotalOutputComponents = 1024;
		resources->maxGeometryUniformComponents = 1024;
		resources->maxGeometryVaryingComponents = 64;
		resources->maxTessControlInputComponents = 128;
		resources->maxTessControlOutputComponents = 128;
		resources->maxTessControlTextureImageUnits = 16;
		resources->maxTessControlUniformComponents = 1024;
		resources->maxTessControlTotalOutputComponents = 4096;
		resources->maxTessEvaluationInputComponents = 128;
		resources->maxTessEvaluationOutputComponents = 128;
		resources->maxTessEvaluationTextureImageUnits = 16;
		resources->maxTessEvaluationUniformComponents = 1024;
		resources->maxTessPatchComponents = 120;
		resources->maxPatchVertices = 32;
		resources->maxTessGenLevel = 64;
		resources->maxViewports = 16;
		resources->maxVertexAtomicCounters = 0;
		resources->maxTessControlAtomicCounters = 0;
		resources->maxTessEvaluationAtomicCounters = 0;
		resources->maxGeometryAtomicCounters = 0;
		resources->maxFragmentAtomicCounters = 8;
		resources->maxCombinedAtomicCounters = 8;
		resources->maxAtomicCounterBindings = 1;
		resources->maxVertexAtomicCounterBuffers = 0;
		resources->maxTessControlAtomicCounterBuffers = 0;
		resources->maxTessEvaluationAtomicCounterBuffers = 0;
		resources->maxGeometryAtomicCounterBuffers = 0;
		resources->maxFragmentAtomicCounterBuffers = 1;
		resources->maxCombinedAtomicCounterBuffers = 1;
		resources->maxAtomicCounterBufferSize = 16384;
		resources->maxTransformFeedbackBuffers = 4;
		resources->maxTransformFeedbackInterleavedComponents = 64;
		resources->maxCullDistances = 8;
		resources->maxCombinedClipAndCullDistances = 8;
		resources->maxSamples = 4;
		resources->maxMeshOutputVerticesNV = 256;
		resources->maxMeshOutputPrimitivesNV = 512;
		resources->maxMeshWorkGroupSizeX_NV = 32;
		resources->maxMeshWorkGroupSizeY_NV = 1;
		resources->maxMeshWorkGroupSizeZ_NV = 1;
		resources->maxTaskWorkGroupSizeX_NV = 32;
		resources->maxTaskWorkGroupSizeY_NV = 1;
		resources->maxTaskWorkGroupSizeZ_NV = 1;
		resources->maxMeshViewCountNV = 4;
		resources->limits.nonInductiveForLoops = 1;
		resources->limits.whileLoops = 1;
		resources->limits.doWhileLoops = 1;
		resources->limits.generalUniformIndexing = 1;
		resources->limits.generalAttributeMatrixVectorIndexing = 1;
		resources->limits.generalVaryingIndexing = 1;
		resources->limits.generalSamplerIndexing = 1;
		resources->limits.generalVariableIndexing = 1;
		resources->limits.generalConstantMatrixVectorIndexing = 1;
	}


	//@―---------------------------------------------------------------------------
	//! @brief  ShaderStages を D3D12_SHADER_VISIBILITY に変換
	//@―---------------------------------------------------------------------------
	static EShLanguage ConvertShaderStage(ShaderStage value) {
		switch (value) {
		case ShaderStage::Vertex:           return EShLangVertex;
		case ShaderStage::Hull:             return EShLangTessControl;
		case ShaderStage::Domain:           return EShLangTessEvaluation;
		case ShaderStage::Geometry:         return EShLangGeometry;
		case ShaderStage::Pixel:            return EShLangFragment;
			//case ShaderStage::Amplification:    return ;
		case ShaderStage::Mesh:             return EShLangMeshNV;
		}

		LOG_WARNING_EX("Graphic", "不正なShaderStage[value={}]", enum_cast(value));
		return EShLangVertex;
	}

	//@―---------------------------------------------------------------------------
	//! @brief  ShaderStages を D3D12_SHADER_VISIBILITY に変換
	//@―---------------------------------------------------------------------------
	const char* GetEntryName(ShaderStage value) {
		switch (value) {
		case ShaderStage::Vertex:           return "VsMain";
		case ShaderStage::Hull:             return "HsMain";
		case ShaderStage::Domain:           return "DsMain";
		case ShaderStage::Geometry:         return "GsMain";
		case ShaderStage::Pixel:            return "PsMain";
			//case ShaderStage::Amplification:    return ;
		case ShaderStage::Mesh:             return "MsMain";
		}

		LOG_WARNING_EX("Graphic", "不正なShaderStage[value={}]", enum_cast(value));
		return "";
	}
	


    //@―---------------------------------------------------------------------------
    //! @brief				シェーダーコードからシェーダーオブジェクトを生成
    //!
    //! @param src			シェーダコード
    //! @param stage		シェーダステージ
    //! @param errorDest	エラー出力先文字列
    //@―---------------------------------------------------------------------------
    ShaderImpl::ShaderImpl(VkDevice device, const String& code, ShaderStage stage)
		: m_device(device)
    {
        // コンパイルできるようにUTF-8にコンバート
        StringBase<char> utfCode;
        StringEncoder::Encode(code, utfCode);

        compile(utfCode, stage);
    }


    //@―---------------------------------------------------------------------------
    //! @brief				バイナリからシェーダーオブジェクトを生成
    //!
    //! @param src			バイナリ
    //! @param stage		シェーダステージ
    //! @param errorDest	エラー出力先文字列
    //@―---------------------------------------------------------------------------
    ShaderImpl::ShaderImpl(VkDevice device, const Blob& blob, ShaderStage stage)
		: m_device(device)
    {
		createShaderModule(blob, stage);
    }

	//@―---------------------------------------------------------------------------
	//! @brief              デストラクタ
	//@―---------------------------------------------------------------------------
    ShaderImpl::~ShaderImpl() {
        if (m_shaderModule)
            ::vkDestroyShaderModule(m_device,m_shaderModule,nullptr);
        m_shaderModule = nullptr;
        m_device = nullptr;
    }


    //@―---------------------------------------------------------------------------
    //! @brief              妥当な状態か
    //@―---------------------------------------------------------------------------
    bool ShaderImpl::isValid()const {
		return m_shaderModule;
    }


    //@―---------------------------------------------------------------------------
    //! @brief  シェーダステージを取得
    //@―---------------------------------------------------------------------------
    ShaderStage ShaderImpl::getStage()const {
        return m_stage;
    }


    //@―---------------------------------------------------------------------------
    //! @brief				初期化
    //@―---------------------------------------------------------------------------
    void ShaderImpl::compile(const StringBase<char>& code, ShaderStage stage) {

        auto const  spirStage = ConvertShaderStage(stage);
		auto const source = code.data();

        glslang::InitializeProcess();

        TBuiltInResource resources = {};
        InitResources(&resources);

        // Enable SPIR-V and Vulkan rules when parsing GLSL
        EShMessages messages = (EShMessages)(EShMsgSpvRules | EShMsgVulkanRules);

        glslang::TShader shader(spirStage);
        shader.setStrings(&source, 1);
        //shader.setEntryPoint("VsMain");

        if (!shader.parse(&resources, 100, false, messages))
        {
			String infoLog, infoDebugLog;
			StringEncoder::Encode(shader.getInfoLog(), infoLog);
			StringEncoder::Encode(shader.getInfoDebugLog(), infoDebugLog);
			LOG_ERROR("ShaderCompileError:\n\t{}\n\t{}", infoLog, infoDebugLog);
			return;
        }

        glslang::TProgram program;
        program.addShader(&shader);

        //
        // Program-level processing...
        //

        if (!program.link(messages))
        {
			String infoLog, infoDebugLog;
			StringEncoder::Encode(shader.getInfoLog(), infoLog);
			StringEncoder::Encode(shader.getInfoDebugLog(), infoDebugLog);
			LOG_ERROR("ShaderCompileError:\n\t{}\n\t{}", infoLog, infoDebugLog);
            return;
        }

        std::vector<unsigned int> spirv;
        //glslang::GlslangToSpv(*program.getIntermediate(spirStage), spirv);

        glslang::FinalizeProcess();

		createShaderModule(BlobView(spirv.data(),sizeof(unsigned int) * spirv.size()), stage);

    }


	void ShaderImpl::createShaderModule(BlobView blob, ShaderStage stage) {

		// チャンクタイプをチェック
		//bool isShaderBlob = *reinterpret_cast<const u32*>(blob.data()) == 0x43425844;
		//if (!isShaderBlob) {
		//	LOG_ERROR_EX("Graphic", "シェーダではないバイナリファイルから構築しようとしました。");
		//}


		VkShaderModuleCreateInfo shaderCreateInfo;
		shaderCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		shaderCreateInfo.pNext = nullptr;
		shaderCreateInfo.flags = 0;
		shaderCreateInfo.codeSize = blob.size();
		shaderCreateInfo.pCode = (uint32_t*)blob.data();

		auto err = ::vkCreateShaderModule(m_device, &shaderCreateInfo, nullptr, &m_shaderModule);
		if (VK_SUCCESS != err) {
			LOG_ERROR("VkShaderModuleの生成に失敗");
			return;
		}

	}

}