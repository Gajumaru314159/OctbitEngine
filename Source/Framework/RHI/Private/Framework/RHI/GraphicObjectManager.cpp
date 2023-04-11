//***********************************************************
//! @file
//! @brief		グラフィック・オブジェクト・マネージャ
//! @author		Gajumaru
//***********************************************************
#include <Framework/RHI/GraphicObjectManager.h>
#include <Framework/RHI/GraphicObject.h>

namespace ob::rhi {


    static GraphicObjectManager* s_graphicObjectManager = nullptr;

    GraphicObjectManager* GraphicObjectManager::Get() {
        return s_graphicObjectManager;
    }

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
        
        OB_ASSERT_EXPR(s_graphicObjectManager == nullptr);
        s_graphicObjectManager = this;
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
        for (auto& pObject : m_objects) {
#ifdef OB_DEBUG
            OB_ABORT("未開放のグラフィックオブジェクト [name={}]", pObject->getName());
#endif
            delete pObject;
        }

        s_graphicObjectManager = nullptr;
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


}// namespace ob::rhi