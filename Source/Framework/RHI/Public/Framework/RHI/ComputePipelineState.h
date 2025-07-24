//***********************************************************
//! @file
//! @brief		コンピュートパイプラインステート
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/GraphicObject.h>
#include <Framework/RHI/Types/ComputePipelineStateDesc.h>

namespace ob::rhi {

    //! @brief      コンピュートパイプラインステート
    class ComputePipelineState :public GraphicObject {
    public:

        //! @brief  生成
        static Ref<ComputePipelineState> Create(const ComputePipelineStateDesc& desc);

    };

}