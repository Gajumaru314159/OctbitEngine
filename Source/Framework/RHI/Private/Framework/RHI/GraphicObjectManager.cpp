//***********************************************************
//! @file
//! @brief		グラフィック・オブジェクト・マネージャ
//! @author		Gajumaru
//***********************************************************
#include <Framework/RHI/GraphicObjectManager.h>
#include <Framework/RHI/GraphicObject.h>

namespace ob::rhi {

    //@―---------------------------------------------------------------------------
    //! @brief  コンストラクタ
    //! 
    //! @param frameCount   削除命令後、何フレーム削除を遅らせるか。
    //@―---------------------------------------------------------------------------
    GraphicObjectManager::GraphicObjectManager(Config* config) 
        : m_config(config ? *config : Config{})
        , m_deleteStackList(std::max(m_config.frameBufferCount,1))
    {
        if (m_config.frameBufferCount < 1) {
            LOG_WARNING("フレーム数が不正です。1以上にしてください。[frameCount={}]", m_config.frameBufferCount);
        }
    }


    //@―---------------------------------------------------------------------------
    //! @brief  デストラクタ
    //@―---------------------------------------------------------------------------
    GraphicObjectManager::~GraphicObjectManager() {

        // フレームバッファ分解放
        for (s32 i = 0; i < std::size(m_deleteStackList); ++i) {
            update();
        }

        // メモリリーク検知
        bool hasMemoryLeak = false;
        for (auto& pObject : m_objects) {
            LOG_ERROR("未開放のグラフィックオブジェクト [name={}]", pObject->getName());
            delete pObject;
        }
        OB_ASSERT(m_objects.empty(), "未開放のグラフィックオブジェクトがあります。");

    }


    //@―---------------------------------------------------------------------------
    //! @brief  更新
    //@―---------------------------------------------------------------------------
    void GraphicObjectManager::update() {

        // インデックスを更新
        m_deleteStackList.next();

        // グラフィック・オブジェクトを削除
        auto& deleteStack = m_deleteStackList.current();
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
        m_objects.push_back(&object);
    }


    //@―---------------------------------------------------------------------------
    //! @brief  解放
    //@―---------------------------------------------------------------------------
    void GraphicObjectManager::requestRelease(GraphicObject& object) {
        m_deleteStackList.current().emplace(&object);
    }


}