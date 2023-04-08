//***********************************************************
//! @file
//! @brief		入力マネージャ
//! @author		Gajumaru
//***********************************************************
#include <Framework/Graphics/Module/GraphicModule.h>
#include <Framework/Engine/Engine.h>
#include <Framework/Engine/ModuleFactory.h>
#include <Framework/RHI/RHIModule.h>

namespace ob::graphics{

    //@―---------------------------------------------------------------------------
    //! @brief  コンストラクタ
    //@―---------------------------------------------------------------------------
    GraphicModule::GraphicModule()
    {
        GEngine->get<rhi::RHIModule>();
    }


    //@―---------------------------------------------------------------------------
    //! @brief  デストラクタ
    //@―---------------------------------------------------------------------------
    GraphicModule::~GraphicModule() = default;



}// namespace ob

REGISTER_MODULE(ob::graphics::GraphicModule);

void Link_GraphicModule() {

}