//***********************************************************
//! @file
//! @brief		入力マネージャ
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Utility/Pimpl.h>

namespace ob::rhi {
    class RHI;
}

namespace ob::graphics {

    class MaterialManager;

    //@―---------------------------------------------------------------------------
    //! @brief  説明
    //@―---------------------------------------------------------------------------
    class GraphicModule: public Singleton<GraphicModule> {
    public:

        //===============================================================
        // コンストラクタ / デストラクタ
        //===============================================================
        GraphicModule(rhi::RHI&);
        ~GraphicModule();

        MaterialManager& getMaterialManager();

    private:
        UPtr<MaterialManager> m_materialManager;

    };

}// namespcae ob