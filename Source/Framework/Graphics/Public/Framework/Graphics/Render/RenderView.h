//***********************************************************
//! @file
//! @brief		
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Misc/YesNo.h>
#include <Framework/RHI/Forward.h>
#include <Framework/Graphics/Forward.h>
#include <Framework/Graphics/CommandBuffer.h>

namespace ob::graphics {

    class RenderContext;
    class CameraData;
    
    //@―---------------------------------------------------------------------------
    //! @brief  
    //@―---------------------------------------------------------------------------
    class RenderView {
    public:

        ~RenderView() = default;

        virtual void render(RenderContext& context, const CameraData& camera) = 0;

    private:



    };


}