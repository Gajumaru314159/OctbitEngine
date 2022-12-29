//***********************************************************
//! @file
//! @brief		グラフィック・オブジェクト
//! @author		Gajumaru
//***********************************************************
#include <Framework/RHI/GraphicObject.h>
#include <Framework/RHI/IGraphicModule.h>
#include <Framework/Engine/Engine.h>

namespace ob::rhi {

    //@―---------------------------------------------------------------------------
    //! @brief  コンストラクタ
    //@―---------------------------------------------------------------------------
    GraphicObject::GraphicObject()
    {
        m_name[0] = TC('\0');

        static GraphicModule* pModule = nullptr;
        if (pModule == nullptr) {
            pModule = GEngine->get<GraphicModule>();
        }
        OB_ASSERT(pModule != nullptr, "Graphicモジュールがありません");

        pModule->getObjectManager().registerObject(*this);
    }

    //@―---------------------------------------------------------------------------
    //! @brief  デストラクタ
    //@―---------------------------------------------------------------------------
    GraphicObject::~GraphicObject() {
    }


    //@―---------------------------------------------------------------------------
    //! @brief  名前を取得
    //@―---------------------------------------------------------------------------
    const Char* GraphicObject::getName()const {
#ifdef OB_DEBUG
        return m_name;
#else
        return TC("Unknown");
#endif
    }


    //@―---------------------------------------------------------------------------
    //! @brief      名前を設定
    //@―---------------------------------------------------------------------------
    void GraphicObject::setName(StringView name) {
#ifdef OB_DEBUG
        OB_ASSERT(name.size() < std::size(m_name), "グラフィック・オブジェクトに設定する名前は{0}文字以下にしてください。[ name={1} ]", std::size(m_name), name.data());
        for (s32 i = 0; i < std::size(m_name) && i < name.size(); ++i) {
            m_name[i] = name[i];
        }
        m_name[name.size()] = TC('\0');
        onNameChanged();
#endif
    }


    //@―---------------------------------------------------------------------------
    //! @brief      終了処理
    //@―---------------------------------------------------------------------------
    void GraphicObject::finalize() {

        static GraphicModule* pModule = nullptr;
        if (pModule == nullptr) {
            pModule = GEngine->get<GraphicModule>();
        }
        OB_ASSERT(pModule != nullptr, "Graphicモジュールがありません");

        pModule->getObjectManager().requestRelease(*this);

    }

}// namespace ob::rhi