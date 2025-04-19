//***********************************************************
//! @file
//! @brief		ルートシグネチャ
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/GraphicObject.h>
#include <Framework/RHI/Types/RenderPassDesc.h>

namespace ob::rhi {

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