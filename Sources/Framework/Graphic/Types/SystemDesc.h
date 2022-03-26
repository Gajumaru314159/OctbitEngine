//***********************************************************
//! @file
//! @brief		システム定義
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphic/Types/GraphicAPI.h>

namespace ob::graphic {

    //@―---------------------------------------------------------------------------
    //! @brief  システム定義
    //@―---------------------------------------------------------------------------
    struct SystemDesc {
        GraphicAPI api;     //!< 描画API
        s32 bufferCount;    //!< レンダリングバッファの数
    };

}// namespcae ob::graphic