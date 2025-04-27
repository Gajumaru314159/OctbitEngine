//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/Shader.h>
#include <Framework/Core/Misc/Blob.h>
#include <Framework/RHI/Types/PipelineStateDesc.h>

namespace ob::rhi {

    //! @brief  シェーダ実装(DirectX12)
    class VulkanShader :public Shader {
    public:

        //! @brief				シェーダーコードからシェーダーオブジェクトを生成
        VulkanShader(VulkanRHI& rhi, const ShaderCompileDesc& desc);


        //! @brief				バイナリからシェーダーオブジェクトを生成
        //!
        //! @param src			バイナリ
        //! @param stage		シェーダステージ
        //! @param errorDest	エラー出力先文字列
        VulkanShader(VulkanRHI& rhi, BlobView blob, ShaderStage stage, StringView name = "Shader");


        //! @brief      名前を取得
        const String& getName()const override;


        //! @brief  シェーダステージを取得
        ShaderStage getStage()const override;

		vk::ShaderModule getNative()const {
			return *m_shaderModule;
		}


    public:

        //! @brief  コンパイル
        void compile(VulkanRHI& rhi, const ShaderCompileDesc& desc);

    private:

        String                  m_name;
        ShaderStage             m_stage;                        //!< シェーダ・ステージ
        Blob                    m_shaderBlob;                   //!< シェーダ・バイナリ
		vk::raii::ShaderModule  m_shaderModule = nullptr;       //!< シェーダ・モジュール

    };

}