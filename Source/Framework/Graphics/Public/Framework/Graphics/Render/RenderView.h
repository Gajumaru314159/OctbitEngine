//***********************************************************
//! @file
//! @brief		
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/Forward.h>
#include <Framework/Graphics/Forward.h>

namespace ob::graphics {

    enum class RenderViewType {
        Camera,
        Shadow,
        ReflectionProbe,
    };

    struct RenderViewDesc {
        String          name;
        Size            size;
        RenderViewType  type;
    };

    enum class RenderViewId{};

    
    //@―---------------------------------------------------------------------------
    //! @brief  
    //@―---------------------------------------------------------------------------
    class RenderView {
        friend class RenderScene;
    public:

        RenderView(RenderViewDesc& desc);
        virtual ~RenderView();

        auto getId()const->RenderViewId;

        //@―---------------------------------------------------------------------------
        //! @brief      名前を取得
        //@―---------------------------------------------------------------------------
        auto& getName()const { return m_desc.name; }

        void setPriority(s32 priority);
        s32 getPriority()const;

        // RenderTexture指定
        // Display設定

    private:
        RenderScene* m_scene;
        RenderViewDesc m_desc;
        RenderViewId m_id;
        s32 m_priority;

    };


}