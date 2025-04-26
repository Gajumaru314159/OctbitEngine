//***********************************************************
//! @file
//! @brief		リソース状態
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/Forward.h>
#include <Framework/RHI/Constants.h>
#include <Framework/RHI/Types/TextureDesc.h>

namespace ob::rhi {


    // enum class AttachmentLoadOp {
    //     Load,
    //     Clear,
    //     DontCare,
    // };
    // enum class AttachmentStoreOp {
    //     Store,
    //     DontCare,
    // };

    enum class RenderPassFlag {
        AllowUAVWrite       = get_bit(0),   //!< RenderPass内でのUAVへの書き込みを許可する
        SuspendingPass      = get_bit(1),   //!< 中断中のレンダーパスである
        ResumingPass        = get_bit(2),   //!< 再開するレンダーパスである
    };
	using RenderPassFlags = BitFlags<RenderPassFlag>;


    enum class RenderPassBeforeAccessType {
        Discard,                // 以前のコンテンツに依存していない。
        Preserve,               // 以前のコンテンツに依存している
        Clear,                  // リソースをクリアする必要がある
        NoAccess,               // 読み取りも書き込みもしない。
    };

    enum class RenderPassAfterAccessType {
        Discard,                // このRenderPass中二書き込んだデータに将来の依存関係がない。
        Preserve,               // 以降のコンテンツが依存している
        NoAccess,               // 読み取りも書き込みもしない。 TODO Vulkanとの対応関係を確認
    };

    struct RenderPassColorTargetDesc {
        Ref<RenderTexture> texture;
		RenderPassBeforeAccessType beforeAccess;
		RenderPassAfterAccessType afterAccess;
    };

    struct RenderPassTargetDesc {
        Ref<RenderTexture> texture;
        RenderPassBeforeAccessType beforeAccess;
        RenderPassAfterAccessType afterAccess;
    };
    using RenderPassTargetDescArray = FixedVector<RenderPassTargetDesc, RENDER_TARGET_MAX>;

    //! レンダーパス定義
    struct RenderPassDesc {
        RenderPassTargetDescArray colors;
        RenderPassTargetDesc depth;
        RenderPassTargetDesc stencil;
        RenderPassFlags flags;
    };

}