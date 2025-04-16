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

    public:

		vk::DescriptorSetLayout getLayouts()const {
			return *m_descriptorSetLayout;
		}

		vk::PipelineLayout getNative() const noexcept
		{
			return *m_pipelineLayout;
		}

        s32 getItemCount(s32 slot) const {
            if (!is_in_range(slot, m_desc.slots)) return false;
            return m_desc.slots[slot].items.size();
        }


    private:

        RootSignatureDesc m_desc;

        Vector<vk::raii::DescriptorSetLayout> m_layouts;

		vk::raii::DescriptorSetLayout m_descriptorSetLayout = nullptr;
		vk::raii::PipelineLayout m_pipelineLayout = nullptr;

    };

    static vk::DescriptorType Convert(BindingType type);

}