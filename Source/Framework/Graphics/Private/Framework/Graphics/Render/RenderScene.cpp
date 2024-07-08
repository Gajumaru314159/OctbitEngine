//***********************************************************
//! @file
//! @brief		
//! @author		Gajumaru
//***********************************************************
#include <Framework/Graphics/Graphics.h>
#include <Framework/Graphics/Render/RenderFeature.h>
#include <Framework/Graphics/Render/RenderScene.h>
#include <Framework/Graphics/Render/RenderView.h>

namespace ob::graphics {

    //@―---------------------------------------------------------------------------
    //! @brief      コンストラクタ
    //@―---------------------------------------------------------------------------
    RenderScene::RenderScene() {
        Graphics::Get()->addScene(this);
    }

    //@―---------------------------------------------------------------------------
    //! @brief      デストラクタ
    //@―---------------------------------------------------------------------------
    RenderScene::~RenderScene() {
        m_releasedNotifier.invoke(*this);

        OB_ASSERT(m_views.empty(), "削除されていないRenderViewが存在します");
        Graphics::Get()->removeScene(this);
    }

    //@―---------------------------------------------------------------------------
    //! @brief      RenderFeatureを見つける
    //@―---------------------------------------------------------------------------
    RenderFeature* RenderScene::findFeature(Type type)const {
        auto found = m_features.find(type);
        if (found == m_features.end())return nullptr;
        return found->second.get();
    }

    //@―---------------------------------------------------------------------------
    //! @brief      ビューを追加
    //@―---------------------------------------------------------------------------
    void RenderScene::addView(RenderView* view) {

        if (view == nullptr) {
            LOG_WARNING("無効なRenderViewは追加できません");
            return;
        }
        if (contains_item(m_views, view)) {
            LOG_WARNING("RenderViewの多重追加はできません");
            return;
        }
        m_views.push_back(view);
    }

    //@―---------------------------------------------------------------------------
    //! @brief      ビューを削除
    //@―---------------------------------------------------------------------------
    void RenderScene::removeView(RenderView* view) {
        if (view == nullptr) {
            LOG_WARNING("無効なRenderViewは削除できません");
            return;
        }
        m_views.erase(std::remove(m_views.begin(), m_views.end(), view), m_views.end());
        //if (!erase_all_item(m_views, view)) {
        //    LOG_WARNING("追加されていないRenderViewを削除しようとしました");
        //    return;
        //}
    }

    //@―---------------------------------------------------------------------------
    //! @brief      解放時イベントを追加
    //@―---------------------------------------------------------------------------
    void RenderScene::addReleasedEvent(RenderSceneEventHandle& handle, RenderSceneEventDelegate func) {
        m_releasedNotifier.add(handle, func);
    }

    //@―---------------------------------------------------------------------------
    //! @brief      RenderFeatureの追加イベントを追加
    //@―---------------------------------------------------------------------------
    void RenderScene::addFeatureAddedEvent(RenderFeatureEventHandle& handle, RenderFeatureEventDelegate func) {
        m_featureAddedNotifier.add(handle, func);
    }

    //@―---------------------------------------------------------------------------
    //! @brief      描画
    //@―---------------------------------------------------------------------------
    void RenderScene::render(FG& fg) {
        for (auto& [type, feature] : m_features) {
            feature->render(fg);
        }
        for (auto view : m_views) {
            view->render(fg);
        }
    }

    //@―---------------------------------------------------------------------------
    //! @brief      RenderFeature追加時
    //@―---------------------------------------------------------------------------
    void RenderScene::onFeatureAdded(RenderFeature& feature) {
        m_featureAddedNotifier.invoke(feature);
    }
}