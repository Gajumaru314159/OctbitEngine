﻿//***********************************************************
//! @file
//! @brief		パイプラインステート
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/Utility/GraphicObjectHolder.h>
#include <Framework/RHI/Types/FrameBufferDesc.h>

namespace ob::rhi {

    //@―---------------------------------------------------------------------------
    //! @brief  パイプラインステート
    //@―---------------------------------------------------------------------------
    class FrameBuffer {
        OB_DEFINE_GRAPHIC_OBJECT_HOLDER(FrameBuffer);
    public:

        //===============================================================
        // コンストラクタ / デストラクタ
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief  コンストラクタ
        //@―---------------------------------------------------------------------------
        FrameBuffer();


        //@―---------------------------------------------------------------------------
        //! @brief  デストラクタ
        //@―---------------------------------------------------------------------------
        FrameBuffer(const FrameBufferDesc& desc);

    };

}// namespcae ob::rhi