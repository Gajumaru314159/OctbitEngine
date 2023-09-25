//***********************************************************
//! @file
//! @brief		
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/Forward.h>
#include <Framework/Graphics/Forward.h>
#include <Framework/Graphics/CommandBuffer.h>
#include <Framework/Graphics/Render/TextureManager.h>

namespace ob::graphics {

    struct RenderViewDesc {
        String  name;
        Size    size;
    };

    
    //@―---------------------------------------------------------------------------
    //! @brief  
    //@―---------------------------------------------------------------------------
    class RenderView {
    public:

        RenderView(RenderViewDesc& desc);
        ~RenderView();

        void setPriority(s32 priority);
        s32 getPriority()const;

    private:

        Name        m_name;

        RenderViewDesc m_desc;

        TextureManager m_textureManager;

    };


}