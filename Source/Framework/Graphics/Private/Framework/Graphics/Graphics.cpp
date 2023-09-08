//***********************************************************
//! @file
//! @brief		システム
//! @author		Gajumaru
//***********************************************************
#include <Framework/Graphics/Graphics.h>
#include <Framework/Graphics/Render/RenderPipeline.h>

#include <Framework/Graphics/Render/RenderContext.h>

namespace ob::graphics {

    Graphics::Graphics() {
        m_renderContext = RenderContext::Create();
    }

    //@―---------------------------------------------------------------------------
    //! @brief      システムをServiceInjectorに登録
    //@―---------------------------------------------------------------------------
    void Graphics::update() {

        if (!m_renderContext)return;
        if (!m_renderPipeline)return;

        

    }

    void Graphics::addCamera(const Ref<Camera>& camera) {
        //m_cameras.push_back(camera);
    }
}