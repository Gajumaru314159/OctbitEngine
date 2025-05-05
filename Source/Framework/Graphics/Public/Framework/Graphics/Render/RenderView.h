//***********************************************************
//! @file
//! @brief		
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/Forward.h>
#include <Framework/RHI/SwapChain.h>
#include <Framework/Graphics/Render/RenderScene.h>
#include <Framework/Graphics/Render/RenderPipeline.h>

namespace ob::graphics {

    //! @brief      レンダー・ビュー
	//! @ref 		RenderView
    class RenderView {
    public:

        //! @brief      コンストラクタ
        RenderView(RenderScene& scene, StringView name);

        //! @brief      デストラクタ
        ~RenderView();

        //! @brief      シーンを取得
        auto getScene() -> RenderScene& { return m_scene; }
        auto getScene() const -> const RenderScene&{ return m_scene; }

        //! @brief      描画矩形を設定
        void setRect(const Rect& rect);

        //! @brief      描画矩形を取得
        auto getRect()const->const Rect&;

        //! @brief      描画サイズを取得
        auto getRenderSize()const->Size;

        //! @brief      描画矩形を取得
        auto getScaledRect()const->IntRect;

        //! @brief      ビューポートを取得
        auto getViewport()const->Viewport;

        //! @brief      描画先ディスプレイを設定する
        void setDisplay(const Ref<SwapChain>& swapChain);

        //! @brief      描画先ディスプレイを取得する
        //! @details    描画先にRenderTextureが設定されている場合は空のオブジェクトが返されます。
        auto getDisplay()const -> const Ref<rhi::SwapChain>;

        //! @brief      描画先テクスチャを設定する
        void setRenderTexture(const Ref<RenderTexture>& renderTexture);

        //! @brief      描画先テクスチャを取得する
        //! @details    描画先にDisplayが設定されている場合は内部的に生成されたRnderTextureが返されます。
        auto getRenderTexture()const->const Ref<rhi::RenderTexture>;

        //! @brief      ビューに使用するRenderPipelineを設定する
        template<class T, class... Args>
        void setPipeline(Args&&... args) {
            m_pipeline = std::make_unique<T>(args...);
        }

        //! @brief      ビューに設定されたRenderPipelineを取得する
        template<class T>
        T* getPipeline() {
            if (!m_pipeline)return nullptr;
            if (m_pipeline->getType() != Type::Get<T>())return nullptr;
            return reinterpret_cast<T*>(m_pipeline.get());
        }

        //! @brief      描画
        void render(FG& fg);

        //! @brief      解放時イベント
        void addReleasedEvent(RenderViewEventHandle& handle, RenderViewEventDelegate func);

    public:

        //! @brief      RenderFeatureを見つける
        template<class T> T* findFeature()const;
        RenderFeature* findFeature(Type type)const;

    private:
        void clearTarget();
        void onSceneReleased(RenderScene& scene);
		void onDisplayUpdated();
    private:
        String                  m_name;
        Rect                    m_rect;
        RenderScene&            m_scene;
        Ref<SwapChain>          m_swapChain;
        SwapChainEventHandle    m_hDisplayUpdated;
        Ref<RenderTexture>      m_renderTexture;
        UPtr<RenderPipeline>    m_pipeline;
        RenderSceneEventHandle  m_hRelease;
        RenderViewEventNotifier m_releasedNotifier;
    };

    template<class T>
    T* RenderView::findFeature()const {
        return reinterpret_cast<T*>(findFeature(Type::Get<T>()));
    }

}