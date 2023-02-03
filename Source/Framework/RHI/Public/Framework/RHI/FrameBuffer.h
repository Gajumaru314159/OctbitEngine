//***********************************************************
//! @file
//! @brief		フレームバッファ
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/GraphicObject.h>
#include <Framework/RHI/Types/FrameBufferDesc.h>

namespace ob::rhi {

    //@―---------------------------------------------------------------------------
    //! @brief      フレームバッファ
    //@―---------------------------------------------------------------------------
    class FrameBuffer :public GraphicObject {
    public:

        //@―---------------------------------------------------------------------------
        //! @brief  生成
        //@―---------------------------------------------------------------------------
        static Ref<FrameBuffer> Create(const FrameBufferDesc& desc);

    public:

        //@―---------------------------------------------------------------------------
        //! @brief  デスクリプタ取得
        //@―---------------------------------------------------------------------------
        virtual const FrameBufferDesc& desc()const = 0;

    protected:

        virtual ~FrameBuffer() = default;

    };

}// namespcae ob::rhi