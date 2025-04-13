//***********************************************************
//! @file
//! @brief		デスクリプタ・テーブル実装(DirectX12)
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/DescriptorTable.h>
#include <Framework/RHI/Texture.h>
#include <Framework/RHI/Buffer.h>
#include <Plugins/VulkanRHI/Descriptor/DescriptorHandle.h>
#include <Plugins/VulkanRHI/RootSignature/RootSignatureImpl.h>

//===============================================================
// クラス定義
//===============================================================
namespace ob::vulkan{
	class Texture;
	class Buffer;
}

namespace ob::rhi::dx12 {
	class DirectX12RHI;
	class DescriptorHeap;
}

//===============================================================
// クラス定義
//===============================================================
namespace ob::rhi::vulkan {

    //! @brief  デスクリプタ・テーブル実装(DirectX12)
    class DescriptorTableImpl :public DescriptorTable {
    public:

        //===============================================================
        //	コンストラクタ
        //===============================================================

        //! @brief              コンストラクタ
        //!
		//! @param device       デバイス
		//! @param type         デスクリプタに設定するリソースの種類
        //! @param elementNum   要素数
        DescriptorTableImpl(VulkanRHI& rhi,DescriptorHeap& heap, const Ref<RootSignature>& signature, s32 slot);
		
		DescriptorTableImpl(VulkanRHI& rhi, DescriptorHeap& heap, const BindingSlot& desc);


		//! @brief      名前を取得
		const String& getName()const override;


		//! @brief  リソースを設定
		//! @{
		bool setResource(s32 index, const Ref<Buffer>& resource) override;
		bool setResource(s32 index, const Ref<Texture>& resource) override;
		bool setResource(s32 index, const Ref<Sampler>& resource) override;
		//! @}

		//! @brief  バインドレスハンドルに使用するインデックスを取得
		u32 getBindlessIndex(s32 index = 0)const override;

	public:

		void record(vk::CommandBuffer commandBuffer, s32 slot) const;

    private:
		VulkanRHI&			m_rhi;

		BindingSlot			m_desc;
		Ref<RootSignatureImpl>  m_signature;
		s32					m_slot = -1;

		String				m_name;


        DescriptorHandle	m_handle;

		using Element = Variant<Ref<Buffer>, Ref<Texture>, Ref<Sampler>>;

		Vector<Element> m_elemetns;

		vk::raii::DescriptorSet m_set = nullptr;

    };

}