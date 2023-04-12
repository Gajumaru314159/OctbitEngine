﻿//***********************************************************
//! @file
//! @brief		レンダーパス
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/GraphicObject.h>
#include <Framework/RHI/Types/RenderPassDesc.h>
#include <Framework/RHI/Types/RenderPassDescHelper.h>

namespace ob::rhi {

    //@―---------------------------------------------------------------------------
    //! @brief      レンダーパス
    //! @details    レンダーパスは複数のサブパスを内包します。
    //!             サブパス間の依存関係を事前に設定することでひとつ前の処理が完了するよりも前に
    //!             次の処理を開始できる場合があります。
    //!             PipelineState はどのRenderPass(Subpass)で描画されるかを知っている必要があります。
    //@―---------------------------------------------------------------------------
    class RenderPass :public GraphicObject {
    public:

        //@―---------------------------------------------------------------------------
        //! @brief  生成
        //@―---------------------------------------------------------------------------
        static Ref<RenderPass> Create(const RenderPassDesc& desc);

    public:

        //@―---------------------------------------------------------------------------
        //! @brief  定義取得
        //@―---------------------------------------------------------------------------
        virtual const RenderPassDesc& desc()const = 0;

    };

}