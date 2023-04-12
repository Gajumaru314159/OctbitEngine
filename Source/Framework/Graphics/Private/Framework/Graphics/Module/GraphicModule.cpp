﻿//***********************************************************
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
    GraphicModule::GraphicModule(rhi::RHI&)
    {
    }


    //@―---------------------------------------------------------------------------
    //! @brief  デストラクタ
    //@―---------------------------------------------------------------------------
    GraphicModule::~GraphicModule() = default;
    MaterialManager& GraphicModule::getMaterialManager() { 
        
        if (!m_materialManager) {
            m_materialManager = std::make_unique<MaterialManager>();
        }
        
        return *m_materialManager.get(); }



}
