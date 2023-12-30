//***********************************************************
//! @file
//! @brief		
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphics/Command/CommandRecorder.h>

namespace ob::graphics {
    
    //@―---------------------------------------------------------------------------
    //! @brief RenderThreadを実行するクラス 
    //@―---------------------------------------------------------------------------
    struct RenderFrameData{
        CommandStorage storage;
    };


}