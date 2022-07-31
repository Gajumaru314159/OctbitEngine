//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include <Framework/Rendering/RenderContext.h>

namespace ob::rendering
{

    //@―---------------------------------------------------------------------------
    //! @brief  説明
    //@―---------------------------------------------------------------------------

    void RenderContext::beginRenderPass(s32 width, s32 height, s32 samles, gsl::span<s32> attachments, s32 depthAttchmentIndex) {

    }
    void RenderContext::endRenderPass() {

    }

    void RenderContext::beginSubPass(gsl::span<s32> colors, gsl::span<s32> inputs, bool isDepthReadOnly) {

    }
    void RenderContext::beginSubPass(gsl::span<s32> colors, bool isDepthReadOnly) {

    }
    void RenderContext::endSubPass() {

    }


    void RenderContext::cull() {

    }
    void RenderContext::drawRenderers(/*cull,settings,filter*/) {

    }


    void RenderContext::executeCummandList() {

    }


}// namespace ob