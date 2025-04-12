//***********************************************************
//! @file
//! @brief		デスクリプタテーブル
//! @author		Gajumaru
//***********************************************************
#include <Framework/RHI/DescriptorTable.h>
#include <Framework/RHI/PipelineState.h>
#include <Framework/RHI/RootSignature.h>
#include <Framework/RHI/RHI.h>

namespace ob::rhi {

    //! @brief              コンストラクタ
    //!
    //! @param type         デスクリプタに設定するリソースの種類
    //! @param elementNum   要素数
    //! @param name         デバッグ名
    Ref<DescriptorTable> DescriptorTable::Create(const Ref<RootSignature>& signature, s32 slot) {
        if (auto rhi = RHI::Get()) {
            return rhi->createDescriptorTable(signature, slot);
        }
        return nullptr;
    }
    Ref<DescriptorTable> DescriptorTable::Create(const PipelineStateDesc& desc, s32 slot) {
        return Create(desc.rootSignature,slot);
    }
    Ref<DescriptorTable> DescriptorTable::Create(const BindingSlot& desc) {
        if (auto rhi = RHI::Get()) {
            return rhi->createDescriptorTable(desc);
        }
        return nullptr;
    }
    //Ref<DescriptorTable> DescriptorTable::Create(const Ref<PipelineState>& pipeline,s32 slot) {
    //	return pipeline ? pipeline->getDesc().rootSignature : nullptr;
    //}

}