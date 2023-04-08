//***********************************************************
//! @file
//! @brief		入力マネージャ
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Engine/IModule.h>
#include <Framework/Graphics/Material/MaterialManager.h>

namespace ob::graphics {

    //@―---------------------------------------------------------------------------
    //! @brief  説明
    //@―---------------------------------------------------------------------------
    class GraphicModule:public engine::IModule{
    public:

        //===============================================================
        // コンストラクタ / デストラクタ
        //===============================================================
        GraphicModule();
        ~GraphicModule();

        MaterialManager& getMaterialManager() { return m_materialManager; }

    private:
        MaterialManager m_materialManager;

    };

}// namespcae ob