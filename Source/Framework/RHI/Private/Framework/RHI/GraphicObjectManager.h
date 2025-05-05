//***********************************************************
//! @file
//! @brief		グラフィック・オブジェクト・マネージャ
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/GraphicObject.h>
#include <Framework/RHI/Config.h>
#include <Framework/Core/Utility/Swapper.h>

namespace ob::rhi {

#define GRAPHIC_OBJECT_LEAK_CHECK_ENABLED OB_DEBUG

    //! @brief  グラフィック・オブジェクト・マネージャ
    class GraphicObjectManager : public Singleton<GraphicObjectManager> {
    public:

        //! @brief  コンストラクタ
        //! 
        //! @param frameCount   削除命令後、何フレーム削除を遅らせるか。
        GraphicObjectManager(RHIConfig*);

        //! @brief  デストラクタ
        ~GraphicObjectManager();

        //! @brief  更新
        void update();

        //! @brief  終了処理
        void finalize();

        //! @brief  登録
        void registerObject(GraphicObject&);

        //! @brief      解放予約
        //! 
        //! @details    指定フレーム更新後に削除するよう予約します。
        void requestRelease(GraphicObject&);

        //! @brief      フレームバッファ数
        s32 getFrameBufferCount()const { return m_config.frameBufferCount; }

    private:

        using ObjectQueue = Queue<GraphicObject*>;
        RHIConfig                  m_config;

        SpinLock                m_deleteStackListLock;
        Swapper<ObjectQueue>    m_deleteStackList;

#if GRAPHIC_OBJECT_LEAK_CHECK_ENABLED
        SpinLock                m_objectsLock;
        List<GraphicObject*>    m_objects;
#endif
    };

}