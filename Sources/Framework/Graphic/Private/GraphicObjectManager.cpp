//***********************************************************
//! @file
//! @brief		グラフィック・オブジェクト・マネージャ
//! @author		Gajumaru
//***********************************************************
#include "GraphicObjectManager.h"
#include <Framework/Graphic/Interface/GraphicObject.h>

namespace ob::graphic {

    //@―---------------------------------------------------------------------------
    //! @brief  コンストラクタ
    //! 
    //! @param frameCount   削除命令後、何フレーム削除を遅らせるか。
    //@―---------------------------------------------------------------------------
    GraphicObjectManager::GraphicObjectManager(s32 frameCount) {
        OB_CHECK_ASSERT_EXPR(0 < frameCount);
        m_deleteStackList.resize(frameCount);
        m_index = 0;
    }


    //@―---------------------------------------------------------------------------
    //! @brief  デストラクタ
    //@―---------------------------------------------------------------------------
    GraphicObjectManager::~GraphicObjectManager() {

        // フレームバッファ分解放
        for (s32 i = 0; i < get_size(m_deleteStackList); ++i) {
            update();
        }

        // メモリリーク検知
        for (auto& pObject : m_objects) {
#ifdef OB_DEBUG
            OB_ASSERT("未開放のグラフィックオブジェクト[ name = {} ]", pObject->getName());
#endif
            delete pObject;
        }
    }


    //@―---------------------------------------------------------------------------
    //! @brief  更新
    //@―---------------------------------------------------------------------------
    void GraphicObjectManager::update() {

        // インデックスを更新
        m_index = (m_index + 1) % get_size(m_deleteStackList);

        // グラフィック・オブジェクトを削除
        auto& deleteStack = m_deleteStackList[m_index];
        while (!deleteStack.empty()) {
            auto pObject = deleteStack.front();
            deleteStack.pop();
            m_objects.remove(pObject);

            delete pObject;
        }
    }


    //@―---------------------------------------------------------------------------
    //! @brief  登録
    //@―---------------------------------------------------------------------------
    void GraphicObjectManager::registerObject(GraphicObject& object) {
        // TODO 登録済みならアサート
        m_objects.push_back(&object);
    }


    //@―---------------------------------------------------------------------------
    //! @brief  解放
    //@―---------------------------------------------------------------------------
    void GraphicObjectManager::requestRelease(GraphicObject& object) {
        OB_CHECK_ASSERT_EXPR(object.getReferenceCount()==0);

        auto& nowStack = m_deleteStackList[m_index];
        nowStack.emplace(&object);
    }


}// namespace ob::graphic