﻿//***********************************************************
//! @file
//! @brief		システム
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphics/Forward.h>
#include <Framework/Core/Utility/Singleton.h>

namespace ob::graphics {

    //@―---------------------------------------------------------------------------
    //! @brief      システムをServiceInjectorに登録
    //@―---------------------------------------------------------------------------
    class Graphics : public Singleton<Graphics>{
    public:

        template<class T,class... Args>
        void setRenderPipeline(Args&&... args) {
            m_renderPipeline = std::make_unique<T>(args...);
        }

        void update();

    public:
        UPtr<RenderPipeline> m_renderPipeline;
    };

}