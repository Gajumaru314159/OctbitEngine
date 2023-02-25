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

//===============================================================
// クラス定義
//===============================================================
namespace ob::rhi{
	class Texture;
	class Buffer;
}

//===============================================================
// クラス定義
//===============================================================
namespace ob::rhi::dx12 {

    //@―---------------------------------------------------------------------------
    //! @brief  デスクリプタ・テーブル実装(DirectX12)
    //@―---------------------------------------------------------------------------
    class DescriptorTableImpl :public DescriptorTable {
    public:

        //===============================================================
        //	コンストラクタ
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief              コンストラクタ
        //!
		//! @param device       デバイス
		//! @param type         デスクリプタに設定するリソースの種類
        //! @param elementNum   要素数
        //@―---------------------------------------------------------------------------
        DescriptorTableImpl(class DescriptorHeap& heap,DescriptorHeapType type, s32 elementNum);


		//@―---------------------------------------------------------------------------
		//! @brief  妥当な状態か
		//@―---------------------------------------------------------------------------
		bool isValid()const override;


		//@―---------------------------------------------------------------------------
		//! @brief      名前を取得
		//@―---------------------------------------------------------------------------
		const String& getName()const override;


		//@―---------------------------------------------------------------------------
		//! @brief  リソースを設定
		//@―---------------------------------------------------------------------------
		//! @{
		bool setResource(s32 index, const Ref<Buffer>& resource) override;
		bool setResource(s32 index, const Ref<Texture>& resource) override;
		//bool setResource(s32 index, class Sampler& resource) override;
		//! @}

		//@―---------------------------------------------------------------------------
		//! @brief  CPUハンドル取得
		//@―---------------------------------------------------------------------------
		D3D12_CPU_DESCRIPTOR_HANDLE getCpuHandle(s32 index = 0)const {
			return m_handle.getCpuHandle(index);
		}

		//@―---------------------------------------------------------------------------
		//! @brief  GPUハンドル取得
		//@―---------------------------------------------------------------------------
		D3D12_GPU_DESCRIPTOR_HANDLE getGpuHandle(s32 index = 0)const {
			return m_handle.getGpuHandle(index);
		}

    private:

		String				m_name;
        DescriptorHandle	m_handle;

		struct Element {
			Ref<Buffer> buffer;

			Ref<Texture> texture;
			TextureEventHandle hTextureUpdate;

			void clear() {
				buffer.reset();
				texture.reset();
				hTextureUpdate.remove();
			}
		};

		Array<UPtr<Element>> m_elemetns;

    };

}// namespcae ob::rhi::dx12