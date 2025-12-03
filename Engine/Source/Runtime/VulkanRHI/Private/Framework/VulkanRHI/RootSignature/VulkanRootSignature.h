//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/RootSignature.h>

namespace ob::rhi {

    //! @brief  ルートシグネチャ実装(Vulkan)
    class VulkanRootSignature :public rhi::RootSignature {
    public:

        //! @brief  コンストラクタ
        VulkanRootSignature(VulkanDevice& device,const RootSignatureDesc& desc);


        //! @brief  デストラクタ
        ~VulkanRootSignature();

        
        //! @brief      名前を取得
        const String& getName()const override;


        //! @brief  定義を取得
        const RootSignatureDesc& getDesc()const noexcept override;

    public:

		vk::PipelineLayout getNative() const noexcept { return *m_pipelineLayout; }

    private:

        RootSignatureDesc m_desc;
		vk::raii::PipelineLayout m_pipelineLayout = nullptr;

    };

}