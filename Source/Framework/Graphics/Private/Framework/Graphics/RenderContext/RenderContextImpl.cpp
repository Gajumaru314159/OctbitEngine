//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include <Framework/Graphics/RenderContext/RenderContextImpl.h>
#include <Framework/Graphics/Material.h>
#include <Framework/Graphics/Camera.h>

#include <Framework/RHI/RenderPass.h>
#include <Framework/RHI/FrameBuffer.h>
#include <Framework/RHI/CommandList.h>

namespace ob::graphics {

    //@―---------------------------------------------------------------------------
    //! @brief      コンストラクタ
    //@―---------------------------------------------------------------------------
    RenderContextImpl::RenderContextImpl() {

        rhi::CommandListDesc desc;
        desc.name = TC("RenderContext");
        desc.type = rhi::CommandListType::Graphic;

        m_commandList = rhi::CommandList::Create(desc);

    }

    //@―---------------------------------------------------------------------------
    //! @brief      RenderPass を開始
    //@―---------------------------------------------------------------------------
    void RenderContextImpl::beginRenderPass(const Ref<rhi::FrameBuffer>& frameBuffer) {
        m_commandList->beginRenderPass(frameBuffer);
    }

    //@―---------------------------------------------------------------------------
    //! @brief      RenderPass を終了
    //@―---------------------------------------------------------------------------
    void RenderContextImpl::endRenderPass() {
        m_commandList->endRenderPass();
    }

    //@―---------------------------------------------------------------------------
    //! @brief      次のサブパスを開始
    //@―---------------------------------------------------------------------------
    void RenderContextImpl::nextSubpass() {
        m_commandList->nextSubpass();
    }

    //@―---------------------------------------------------------------------------
    //! @brief      カスタムコマンドバッファーを記録
    //! 
    //! @details    この関数を呼び出すと、RenderContext内部のCommandBufferにカスタムCommandBufferの内容がコピーされます。
    //!             呼出し後にカスタムCommandBufferを再利用しない場合はCommandBufferをクリアしてください。
    //@―---------------------------------------------------------------------------
    void RenderContextImpl::executeCustomCommand(const CommandBuffer&) {

        // CommandListを分割して挿入？

    }


    //@―---------------------------------------------------------------------------
    //! @brief      カメラのプロパティをグローバル変数に書き込む
    //@―---------------------------------------------------------------------------
    void RenderContextImpl::setCamera(const Camera& camera) {

        // TODO カメラ毎に生成？
        // サイズ的にはRootConstantでよさそう
        Material::SetGlobalMatrix(TC("ViewMatrix"), camera.getViewMatrix());
        Material::SetGlobalMatrix(TC("ProjectionMatrix"), camera.getProjectionMatrix());
        Material::SetGlobalMatrix(TC("ViewProjectionMatrix"), camera.getViewProjectionMatrix());

    }

    //@―---------------------------------------------------------------------------
    //! @brief      特定の描画タグを持つ描画アイテムを描画する
    //@―---------------------------------------------------------------------------
    //RenderGroup getRendererGroup(Name renderTag) const;// Debug / Shadow / UI

    //@―---------------------------------------------------------------------------
    //! @brief      特定の描画タグを持つ描画アイテムを描画する
    //@―---------------------------------------------------------------------------
    void RenderContextImpl::draw() {

        // TODO 設計

    }

    //@―---------------------------------------------------------------------------
    //! @brief      記録された全てのコマンドを実行
    //! 
    //! @details    記録されているコマンドを即時実行し、実行が終わるまで待機します。
    //@―---------------------------------------------------------------------------
    void RenderContextImpl::submit() {

        // 分割している場合の対応
        m_commandList->flush();

    }

}