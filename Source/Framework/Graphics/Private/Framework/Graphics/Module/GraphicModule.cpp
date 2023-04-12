//***********************************************************
//! @file
//! @brief		入力マネージャ
//! @author		Gajumaru
//***********************************************************
#include <Framework/Graphics/GraphicModule.h>
#include <Framework/Graphics/Material/MaterialManager.h>
#include <Framework/Engine/Engine.h>
#include <Framework/RHI/RHI.h>

namespace ob::graphics{

    //@―---------------------------------------------------------------------------
    //! @brief  コンストラクタ
    //@―---------------------------------------------------------------------------
    GraphicModule::GraphicModule(rhi::RHI&, MaterialManager&)
    {
    }


    //@―---------------------------------------------------------------------------
    //! @brief  デストラクタ
    //@―---------------------------------------------------------------------------
    GraphicModule::~GraphicModule() = default;

}
