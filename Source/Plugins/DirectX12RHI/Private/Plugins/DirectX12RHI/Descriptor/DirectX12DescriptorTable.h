//***********************************************************
//! @file
//! @brief		デスクリプタ・テーブル実装(DirectX12)
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/DescriptorTable.h>
#include <Framework/RHI/Texture.h>
#include <Framework/RHI/Buffer.h>
#include <Plugins/DirectX12RHI/Descriptor/DescriptorHandle.h>
#include <Plugins/DirectX12RHI/RootSignature/DirectX12RootSignature.h>

//===============================================================
// クラス定義
//===============================================================
namespace ob::rhi{
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
namespace ob::rhi::dx12 {

    //! @brief  デスクリプタ・テーブル実装(DirectX12)
    class DirectX12DescriptorTable :public DescriptorTable {
    public:

        //===============================================================
        //	コンストラクタ
        //===============================================================

        //! @brief              コンストラクタ
        //!
		//! @param device       デバイス
		//! @param type         デスクリプタに設定するリソースの種類
        //! @param elementNum   要素数
        DirectX12DescriptorTable(DirectX12RHI& device,DescriptorHeap& heap, const Ref<RootSignature>& signature, s32 slot);
		
		DirectX12DescriptorTable(DirectX12RHI& device, DescriptorHeap& heap, const BindingSlot& desc);



		//! @brief  妥当な状態か
		bool isValid()const;


		//! @brief      名前を取得
		const String& getName()const override;


		//! @brief  リソースを設定
		//! @{
		bool setResource(s32 index, const Ref<Buffer>& resource) override;
		bool setResource(s32 index, const Ref<Texture>& resource) override;
		bool setResource(s32 index, const Ref<Sampler>& resource) override;
		//! @}

		//! @brief  CPUハンドル取得
		D3D12_CPU_DESCRIPTOR_HANDLE getCpuHandle(s32 index = 0)const {
			return m_handle.getCpuHandle(index);
		}

		//! @brief  GPUハンドル取得
		D3D12_GPU_DESCRIPTOR_HANDLE getGpuHandle(s32 index = 0)const {
			return m_handle.getGpuHandle(index);
		}

		//! @brief  バインドレスハンドルに使用するインデックスを取得
		u32 getBindlessIndex(s32 index = 0)const override {
			return m_handle.getBindlessIndex(index);
		}

	public:

		void record(ID3D12GraphicsCommandList& cmdList,s32 slot) const;

	private:

		bool tryGetRangeType(s32 index, const Ref<rhi::Buffer>& buffer, D3D12_DESCRIPTOR_RANGE_TYPE& type) const;
		bool tryGetRangeType(s32 index, const Ref<rhi::Texture>& texture, D3D12_DESCRIPTOR_RANGE_TYPE& type) const;
		bool tryGetRangeType(s32 index, const Ref<rhi::Sampler>& sampler, D3D12_DESCRIPTOR_RANGE_TYPE& type) const;

    private:
		DirectX12RHI&		m_rhi;

		BindingSlot			m_desc;
		Ref<DirectX12RootSignature>  m_signature;
		s32					m_slot = -1;

		String				m_name;
        DescriptorHandle	m_handle;

		using Element = Variant<Ref<Buffer>, Ref<Texture>, Ref<Sampler>>;

		Vector<Element> m_elemetns;

    };

}