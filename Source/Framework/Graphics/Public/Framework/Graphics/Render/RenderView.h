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

    enum class RenderViewType {
        Camera,
        Shadow,
        ReflectionCube,
    };

    struct RenderViewDesc {
        String          name;
        Size            size;
        RenderViewType  type;
    };

    
    //@―---------------------------------------------------------------------------
    //! @brief  
    //@―---------------------------------------------------------------------------
    class RenderView {
    public:

        RenderView(RenderViewDesc& desc);
        virtual ~RenderView();

        void setPriority(s32 priority);
        s32 getPriority()const;

        void render(CommandRecorder&);

        template<class T>
        void addStep();
        void addStep(UPtr<RenderStep>);
        // addDrawPacket
        // addDrawItem

    private:

        RenderViewDesc m_desc;

        UPtr<TextureManager> m_textureManager;

        Array<UPtr<RenderStep>> m_steps;

    };


}