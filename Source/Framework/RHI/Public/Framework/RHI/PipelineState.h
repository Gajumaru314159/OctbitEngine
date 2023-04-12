//***********************************************************
//! @file
//! @brief		パイプラインステート
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/GraphicObject.h>
#include <Framework/RHI/Types/PipelineStateDesc.h>

namespace ob::rhi {

    //@―---------------------------------------------------------------------------
    //! @brief      パイプラインステート
    //@―---------------------------------------------------------------------------
    class PipelineState :public GraphicObject {
    public:

        //@―---------------------------------------------------------------------------
        //! @brief  生成
        //@―---------------------------------------------------------------------------
        static Ref<PipelineState> Create(const PipelineStateDesc& desc);

    };

}