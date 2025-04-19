//***********************************************************
//! @file
//! @brief		リソース状態
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/Types/TextureDesc.h>

namespace ob::rhi {

    enum class AttachmentLoadOp {
        Load,
        Clear,
        DontCare,
    };
    enum class AttachmentStoreOp {
        Store,
        DontCare,
    };

    struct AttackmentDesc {
        TextureFormat format;

        // 以下はできれば隠ぺいしたい
        AttachmentLoadOp load;
        AttachmentStoreOp store;
        // ステンシル
        TextureState initialState;
        TextureState finalState;
    };

    //! レンダーパス
    //! レンダーパスは描画
    struct RenderPassDesc {
        Vector<AttackmentDesc> attachments;
    };

}