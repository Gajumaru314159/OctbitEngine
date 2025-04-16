//***********************************************************
//! @file
//! @brief		ルートシグネチャ
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/GraphicObject.h>
#include <Framework/RHI/Types/RootSignatureDesc.h>
#include <Framework/RHI/Types/TextureFormat.h>
#include <Framework/RHI/Types/ResourceState.h>

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
		ResourceState initialState; 
		ResourceState finalState;
    };

	//! レンダーパス
    //! レンダーパスは描画
    struct RenderPassDesc {
		Vector<AttackmentDesc> attachments;
    };



    //! レンダーパス
    //! 
    //! 描画ターゲットのフォーマット及び描画操作を事前設定するためのオブジェクトです。
	//! FrameBuffer を生成する際に必要となります。
    class RenderPass :public GraphicObject {
    public:
        
        //! @brief  生成
        static Ref<RenderPass> Create(const RenderPassDesc& desc);

    public:

    };

}