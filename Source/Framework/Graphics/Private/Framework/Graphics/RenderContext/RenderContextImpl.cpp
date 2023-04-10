﻿//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
//#include <Framework/Graphics/RenderContext/RenderContextImpl.h>

namespace ob::graphics {

    /*


    //@―---------------------------------------------------------------------------
    //! @brief      コンストラクタ
    //@―---------------------------------------------------------------------------
    RenderContextImpl::RenderContextImpl() {
    }

    //@―---------------------------------------------------------------------------
    //! @brief      RenderPass を開始
    //@―---------------------------------------------------------------------------
    void RenderContextImpl::beginRenderPass(s32 width, s32 height, Span<Attachment> attachments, std::optional<Attachment> depth) {
        //if (m_renderPassCache) {
        //    endRenderPass();
        //}
        //m_renderPassCache = createRenderPassCache(width,height,attachments,depthIndex);

        OB_NOTIMPLEMENTED();
    }

    //@―---------------------------------------------------------------------------
    //! @brief      サブパスを開始
    //@―---------------------------------------------------------------------------
    void RenderContextImpl::beginSubPass(Span<s32> colors, Span<s32> inputs) {
        //if (!m_renderPassCache)
        //    return:
        //
        //m_renderPassCache->addSubpass(colors,inputs);
        OB_NOTIMPLEMENTED();
    }


    //@―---------------------------------------------------------------------------
    //! @brief      カメラのプロパティをグローバル変数に書き込む
    //@―---------------------------------------------------------------------------
    void RenderContextImpl::setCamera(const Camera& camera) {
        OB_NOTIMPLEMENTED();
    }

    //@―---------------------------------------------------------------------------
    //! @brief      特定の描画タグを持つ描画アイテムを描画する
    //@―---------------------------------------------------------------------------
    RenderGroup RenderContextImpl::getRendererGroup(engine::Name renderTag) const {
        return {};
    }

    //@―---------------------------------------------------------------------------
    //! @brief      特定の描画タグを持つ描画アイテムを描画する
    //@―---------------------------------------------------------------------------
    void RenderContextImpl::draw(tag,sort,filter) {

        //auto& items = m_itemMap[tag]{}
        //
        //    std::sort(items.begin(), items.end(), m_sorter[sort]) {}
        //
        //for (auto& item : items) {
        //    if (!item.getLayers().has(layer))continue {}
        //
        //    item.render(m_cmdBuffer, tag) {}
        //
        //}

        OB_NOTIMPLEMENTED();
    }

    //@―---------------------------------------------------------------------------
    //! @brief      カスタムコマンドバッファーを記録
    //! 
    //! @details    この関数を呼び出すと、RenderContextImpl内部のCommandBufferにカスタムCommandBufferの内容がコピーされます。
    //!             呼出し後にカスタムCommandBufferを再利用しない場合はCommandBufferをクリアしてください。
    //@―---------------------------------------------------------------------------
    void RenderContextImpl::executeCustomCommand(const CommandBuffer&) {
        OB_NOTIMPLEMENTED();
    }

    //@―---------------------------------------------------------------------------
    //! @brief      
    //@―---------------------------------------------------------------------------
    void RenderContextImpl::invokeOnRenderObjectCallBack() {
        OB_NOTIMPLEMENTED();
    }


    //@―---------------------------------------------------------------------------
    //! @brief      サブパスを終了
    //@―---------------------------------------------------------------------------
    void RenderContextImpl::endSubPass() {
        OB_NOTIMPLEMENTED();
    }

    //@―---------------------------------------------------------------------------
    //! @brief      RenderPass を終了
    //@―---------------------------------------------------------------------------
    void RenderContextImpl::endRenderPass() {
        //m_renderPassCache->flush();
        OB_NOTIMPLEMENTED();
    }

    //@―---------------------------------------------------------------------------
    //! @brief      記録された全てのコマンドを実行
    //! 
    //! @details    記録されているコマンドを即時実行し、実行が終わるまで待機します。
    //@―---------------------------------------------------------------------------
    void RenderContextImpl::submit() {
        OB_NOTIMPLEMENTED();
    }
    */

}// namespace ob