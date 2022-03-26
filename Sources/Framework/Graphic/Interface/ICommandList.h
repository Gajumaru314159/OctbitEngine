//***********************************************************
//! @file
//! @brief		コマンドリスト・インターフェイス
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphic/Interface/GraphicObject.h>

//===============================================================
// 前方宣言
//===============================================================
namespace ob::graphic {
}

//===============================================================
// クラス宣言
//===============================================================
namespace ob::graphic {

    //@―---------------------------------------------------------------------------
    //! @brief      コマンドリスト・インターフェイス
    //@―---------------------------------------------------------------------------
    class ICommandList :public GraphicObject {
    public:

        virtual void beginRender();
        virtual void endRender();

        virtual void clearRenderTarget(const Color& color, const s32 target);
        virtual void clearDepthStencil(const f32 depth, const u32 stencil);

        //void setScissorRect(const IntRect& rect);
        //void setViewport();

        virtual void setVertexBuffer() = 0;
        virtual void setIndexBuffer() = 0;

        virtual void setPipelineState() = 0;

        virtual void setBlendFactor() = 0;
        virtual void setStencilRef() = 0;

        virtual void setRootDesciptor() = 0;

        virtual void draw() = 0;
        virtual void drawIndexed() = 0;

        // インスタンシング描画
        // 間接引数描画

        virtual void dispatch() = 0;

        // バッファのコピー
        // リージョンコピー

        virtual void copyTexture() = 0;

        virtual void insertResourceBarrier() = 0;



    };

}// namespace pb::graphic