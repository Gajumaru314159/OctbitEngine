﻿//***********************************************************
//! @file
//! @brief		デスクリプタ・テーブル実装(DirectX12)
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphic/Interface/IDescriptorTable.h>
#include <Plugins/GraphicDirectX12/Descriptor/DescriptorHandle.h>

//===============================================================
// クラス定義
//===============================================================
namespace ob::graphic::dx12 {

    //@―---------------------------------------------------------------------------
    //! @brief  デスクリプタ・テーブル実装(DirectX12)
    //@―---------------------------------------------------------------------------
    class DescriptorTableImpl :public IDescriptorTable {
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
		//! @brief  リソースを設定
		//@―---------------------------------------------------------------------------
		//! @{
		bool setResource(s32 index, class Buffer& resource) override;
		bool setResource(s32 index, Texture& resource) override;
		//bool setResource(s32 index, class Sampler& resource) override;
		//! @}


		//@―---------------------------------------------------------------------------
		//! @brief  リソースのバインドを解除
		//@―---------------------------------------------------------------------------
		void clear() override;


		//@―---------------------------------------------------------------------------
		//! @brief  指定したインデックスのリソースのバインドを解除
		//@―---------------------------------------------------------------------------
		void clearAt(s32 index) override;

		D3D12_CPU_DESCRIPTOR_HANDLE getCpuHandle(s32 index = 0)const {
			return m_handle.getCpuHandle(index);
		}

		D3D12_GPU_DESCRIPTOR_HANDLE getGpuHandle(s32 index = 0)const {
			return m_handle.getGpuHandle(index);
		}

    private:

        DescriptorHandle m_handle;

    };

}// namespcae ob::graphic::dx12