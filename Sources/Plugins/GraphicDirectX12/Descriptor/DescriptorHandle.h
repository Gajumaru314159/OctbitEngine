//***********************************************************
//! @file
//! @brief		デスクリプタ・ハンドル
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphic/Types/DescriptorDesc.h>

namespace ob::graphic::dx12 {

    //@―---------------------------------------------------------------------------
    //! @brief  説明
    //@―---------------------------------------------------------------------------
    class DescriptorHandle:private Noncopyable {
        friend class DescriptorHeap;
    public:

        //===============================================================
        // コンストラクタ / デストラクタ
        //===============================================================

        DescriptorHandle();
        ~DescriptorHandle();

        DescriptorHandle(DescriptorHandle&&);
        DescriptorHandle& operator = (DescriptorHandle&&);

        bool empty()const noexcept;
        void release();
        DescriptorHeapType getHeapType()const;

        D3D12_CPU_DESCRIPTOR_HANDLE getCpuHandle(s32 index = 0);
        D3D12_GPU_DESCRIPTOR_HANDLE getGpuHandle(s32 index = 0);

    private:

        struct BBlock* m_pBlock;

    };

}// namespcae ob::graphic::dx12