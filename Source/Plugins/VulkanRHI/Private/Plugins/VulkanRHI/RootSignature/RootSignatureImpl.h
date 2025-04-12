//***********************************************************
//! @file
//! @brief		ルートシグネチャ実装(DirectX12)
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/RootSignature.h>

//===============================================================
// クラス宣言
//===============================================================
namespace ob::rhi::vulkan {

    //! @brief  ルートシグネチャ実装(Vulkan)
    class RootSignatureImpl :public rhi::RootSignature {
    public:

        //===============================================================
        // コンストラクタ / デストラクタ
        //===============================================================

        //! @brief  コンストラクタ
        RootSignatureImpl(VulkanRHI& rhi,const RootSignatureDesc& desc);


        //! @brief  デストラクタ
        ~RootSignatureImpl();

        
        //! @brief      名前を取得
        const String& getName()const override;


        //===============================================================
        // ゲッター
        //===============================================================

        //! @brief  定義を取得
        const RootSignatureDesc& getDesc()const noexcept override;




    private:

        const RootSignatureDesc m_desc;

        Vector<vk::raii::DescriptorSetLayout> m_layouts;

		vk::raii::DescriptorSetLayout m_descriptorSetLayout = nullptr;
		vk::raii::PipelineLayout m_pipelineLayout = nullptr;

    };

    static vk::DescriptorType Convert(DescriptorRangeType type);

}