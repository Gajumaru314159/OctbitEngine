﻿//***********************************************************
//! @file
//! @brief		グラフィックAPIタイプ
//! @author		Gajumaru
//***********************************************************
#pragma once

namespace ob {
    namespace graphic {

        //@―---------------------------------------------------------------------------
        //! @brief      グラフィックAPIタイプ
        //! 
        //! @details    |Windows|Linux|
        //!             |-------|-----|
        //!             |DirectX|Vulkan|
        //!             |Vulkan||
        //@―---------------------------------------------------------------------------
        enum class GraphicAPI {
            D3D12,
            Vulkan,
        };

    }
}// namespcae ob