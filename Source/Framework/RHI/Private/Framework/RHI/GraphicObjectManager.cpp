//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#include <Framework/RHI/GraphicObjectManager.h>
#include <Framework/RHI/GraphicObject.h>

namespace ob::rhi {

    //! @brief  コンストラクタ
    GraphicObjectManager::GraphicObjectManager(const RHIConfig* config)
        : m_config(config ? *config : RHIConfig{})
        , m_deleteStackList(std::max(m_config.frameBufferCount+1,1)) // 描画遅延+解放中の書き込み先
    {
        if (m_config.frameBufferCount < 1) {
            LOG_WARNING("フレーム数が不正です。1以上にしてください。[frameCount={}]", m_config.frameBufferCount);
        }
    }


    //! @brief  デストラクタ
    GraphicObjectManager::~GraphicObjectManager() {

        // フレームバッファ分解放
        for (s32 i = 0; i < std::size(m_deleteStackList); ++i) {
            update();
        }

#if GRAPHIC_OBJECT_LEAK_CHECK_ENABLED
        // メモリリーク検知
		ScopeLock lock(m_objectsLock);
        for (auto& pObject : m_objects) {
            LOG_ERROR("未開放のグラフィックオブジェクト [name={}]", pObject->getName());
            delete pObject;
        }
        OB_ASSERT(m_objects.empty(), "未開放のグラフィックオブジェクトがあります。");
#endif
    }


    //! @brief  更新
    void GraphicObjectManager::update() {

        // インデックスを更新
        {
            ScopeLock lock(m_deleteStackListLock);
            m_deleteStackList.next();
        }

        // グラフィック・オブジェクトを削除
        auto& deleteStack = m_deleteStackList.at((m_deleteStackList.index()+1)% m_deleteStackList.size());

        while (!deleteStack.empty()) {
            auto pObject = deleteStack.front();
            deleteStack.pop();

#if GRAPHIC_OBJECT_LEAK_CHECK_ENABLED
            {
                ScopeLock lock(m_objectsLock);
                m_objects.remove(pObject);
            }
#endif

            delete pObject;
        }
    }


    //! @brief  更新
    void GraphicObjectManager::finalize() {
		// DescriptorTable -> TextureView -> Texture のような参照関係が深いオブジェクトもあるので多めに待機する
        for (s32 i = 0; i < 100;++i) {
            update();
            ScopeLock lock(m_deleteStackListLock);
		    size_t deletingCount = 0;
		    for (s32 j = 0; j < m_deleteStackList.size(); ++j) {
			    deletingCount += m_deleteStackList.at(j).size();
		    }
            if (deletingCount == 0) {
                break;
            }
        }
    }


    //! @brief  登録
    void GraphicObjectManager::registerObject(GraphicObject& object) {
#if GRAPHIC_OBJECT_LEAK_CHECK_ENABLED
        ScopeLock lock(m_objectsLock);
        m_objects.push_back(&object);
#endif
    }


    //! @brief  解放
    void GraphicObjectManager::requestRelease(GraphicObject& object) {
		ScopeLock lock(m_deleteStackListLock);
        m_deleteStackList.current().emplace(&object);
    }


}