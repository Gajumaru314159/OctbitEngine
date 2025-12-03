//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/GraphicObject.h>
#include <Framework/RHI/Types/SamplerDesc.h>
#include <Framework/RHI/Types/BindlessHandle.h>

namespace ob::rhi {

    //! @brief      サンプラー
    class Sampler :public GraphicObject {
    public:
        static Ref<Sampler> Default();
    public:

        static Ref<Sampler> Create(const SamplerDesc& desc);

    public:

        //! @brief      BindlessHandleを取得
        virtual BindlessHandle getHandle()const = 0;

    };

}

