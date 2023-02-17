﻿//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphic/RenderContext.h>
#include <Framework/RHI/CommandList.h>
#include <Framework/Graphic/Camera.h>

namespace ob::graphic {
    /*
    //@―---------------------------------------------------------------------------
    //! @brief  
    //! 
    //! @details    内部的にCommandBufferを持っています。独自の描画方法を使用する場合は
    //!             CommandBufferに直接コマンドを記録し、executeCommandBuffer()を記録してください。
    //!             RenderContextはRenderPassの構築を内部的に処理するために、すべての描画処理を
    //!             endRenderPassで行います。それ以前の描画コマンドはLinearAllocatorに追加していきます。
    //@―---------------------------------------------------------------------------
    class RenderContextImpl:public RenderContext{
    public:

        //===============================================================
        // コンストラクタ / デストラクタ
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief      コンストラクタ
        //@―---------------------------------------------------------------------------
        RenderContextImpl();


        //@―---------------------------------------------------------------------------
        //! @brief      RenderPass を開始
        //@―---------------------------------------------------------------------------
        void beginRenderPass(s32 width, s32 height, Span<Attachment> attachments, std::optional<Attachment> depth)override;

        //@―---------------------------------------------------------------------------
        //! @brief      サブパスを開始
        //@―---------------------------------------------------------------------------
        void beginSubPass(Span<s32> colors, Span<s32> inputs)override;


        //@―---------------------------------------------------------------------------
        //! @brief      カメラのプロパティをグローバル変数に書き込む
        //@―---------------------------------------------------------------------------
        void setCamera(const Camera& camera)override;

        //@―---------------------------------------------------------------------------
        //! @brief      特定の描画タグを持つ描画アイテムを描画する
        //@―---------------------------------------------------------------------------
        void draw()override;// Debug / Shadow / UI

        //@―---------------------------------------------------------------------------
        //! @brief      特定の描画タグを持つ描画アイテムを描画する
        //@―---------------------------------------------------------------------------
        RenderGroup getRendererGroup(engine::Name renderTag) const override;// Debug / Shadow / UI

        //@―---------------------------------------------------------------------------
        //! @brief      カスタムコマンドバッファーを記録
        //! 
        //! @details    この関数を呼び出すと、RenderContext内部のCommandBufferにカスタムCommandBufferの内容がコピーされます。
        //!             呼出し後にカスタムCommandBufferを再利用しない場合はCommandBufferをクリアしてください。
        //@―---------------------------------------------------------------------------
        void executeCustomCommand(const CommandBuffer&)override;

        //@―---------------------------------------------------------------------------
        //! @brief      
        //@―---------------------------------------------------------------------------
        void invokeOnRenderObjectCallBack()override;


        //@―---------------------------------------------------------------------------
        //! @brief      サブパスを終了
        //@―---------------------------------------------------------------------------
        void endSubPass()override;

        //@―---------------------------------------------------------------------------
        //! @brief      RenderPass を終了
        //@―---------------------------------------------------------------------------
        void endRenderPass()override;

        //@―---------------------------------------------------------------------------
        //! @brief      記録された全てのコマンドを実行
        //! 
        //! @details    記録されているコマンドを即時実行し、実行が終わるまで待機します。
        //@―---------------------------------------------------------------------------
        void submit()override;

    private:

        Ref<rhi::CommandList> m_commandList;

    };
*/

}// namespcae ob