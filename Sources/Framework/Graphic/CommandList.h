﻿//***********************************************************
//! @file
//! @brief		コマンドリスト
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphic/Utility/GraphicObjectHolder.h>
#include <Framework/Graphic/Types/CommandListDesc.h>
#include <Framework/Core/Geometory/Viewport.h>

namespace ob::graphic {

    //@―---------------------------------------------------------------------------
    //! @brief      コマンドリスト
    //@―---------------------------------------------------------------------------
    class CommandList {
        OB_DEFINE_GRAPHIC_OBJECT_HOLDER(CommandList);
    public:

        //===============================================================
        // コンストラクタ / デストラクタ
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief  コンストラクタ
        //@―---------------------------------------------------------------------------
        CommandList();


        //@―---------------------------------------------------------------------------
        //! @brief  コンストラクタ
        //@―---------------------------------------------------------------------------
        CommandList(const CommandListDesc& desc, StringView name=TC("CommandList"));


        //@―---------------------------------------------------------------------------
        //! @brief  コマンドをシステムキューに追加
        //@―---------------------------------------------------------------------------
        void flush();


        //===============================================================
        // コマンド
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief      記録開始
        //@―---------------------------------------------------------------------------
        void begin();

        //@―---------------------------------------------------------------------------
        //! @brief      記録終了
        //@―---------------------------------------------------------------------------
        void end();

        //@―---------------------------------------------------------------------------
        //! @brief      書き込み先をスワップチェーンに設定
        //@―---------------------------------------------------------------------------
        void setRenderTarget(const class SwapChain& target);

        //@―---------------------------------------------------------------------------
        //! @brief      書き込み先をレンダーターゲットに設定
        //@―---------------------------------------------------------------------------
        void setRenderTarget(const class RenderTarget& target);

        //@―---------------------------------------------------------------------------
        //! @brief      シザー矩形を設定
        //! 
        //! @details    ビューポートのどの部分に描画するか。
        //!             値はピクセル指定で、どの矩形を使用するかはジオメトリシェーダで指定できます。
        //@―---------------------------------------------------------------------------
        void setScissorRect(const IntRect* pRect, s32 num);

        //@―---------------------------------------------------------------------------
        //! @brief      ビューポートを設定
        //! 
        //! @details    描画ターゲットのどの範囲に書き込むか。
        //!             Z値以外はピクセル指定で、どのビューポートを使用するかはジオメトリシェーダで指定できます。
        //@―---------------------------------------------------------------------------
        void setViewport(const Viewport* pViewport, s32 num);

        //@―---------------------------------------------------------------------------
        //! @brief      レンダーターゲットの色をRenderTargetに設定した色でクリア
        //@―---------------------------------------------------------------------------
        void clearColors(u32 mask = -1);

        //@―---------------------------------------------------------------------------
        //! @brief      レンダーターゲットのデプスとステンシルをクリア
        //@―---------------------------------------------------------------------------
        void clearDepthStencil();

        //@―---------------------------------------------------------------------------
        //! @brief      ルートシグネチャを設定
        //@―---------------------------------------------------------------------------
        void setRootSignature(const class RootSignature& pipeline);

        //@―---------------------------------------------------------------------------
        //! @brief      描画パイプラインを設定
        //@―---------------------------------------------------------------------------
        void setPipelineState(const class PipelineState& pipeline);

        //@―---------------------------------------------------------------------------
        //! @brief      頂点バッファを設定
        //@―---------------------------------------------------------------------------
        void setVertexBuffer(const class Buffer& buffer);

        //@―---------------------------------------------------------------------------
        //! @brief      インデックスバッファを設定
        //@―---------------------------------------------------------------------------
        void setIndexBuffer(const class Buffer& buffer);

        //@―---------------------------------------------------------------------------
        //! @brief      デスクリプタテーブルを設定
        //@―---------------------------------------------------------------------------
        void setRootDesciptorTable(const struct SetDescriptorTableParam*, s32 num);

        //@―---------------------------------------------------------------------------
        //! @brief      インデックスありで描画
        //@―---------------------------------------------------------------------------
        void drawIndexedInstanced();

        //@―---------------------------------------------------------------------------
        //! @brief      インデックスなしで描画
        //@―---------------------------------------------------------------------------
        void drawInstanced();

    };


}// namespace pb::graphic