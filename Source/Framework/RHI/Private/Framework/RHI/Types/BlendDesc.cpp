//***********************************************************
//! @file
//! @brief		ブレンド定義
//! @author		Gajumaru
//***********************************************************
#include <Framework/RHI/Types/BlendDesc.h>

namespace ob::rhi {

#define SHARED_STATE        \
        BlendFactor::One,   \
        BlendFactor::Zero,  \
        BlendOp::Add,       \
        ColorCompoent::All


    const BlendDesc BlendDesc::None = {
        false,
        BlendFactor::One,
        BlendFactor::Zero,
        BlendOp::Add,
        SHARED_STATE
    };

    const BlendDesc BlendDesc::AlphaBlend = {
        true,
        BlendFactor::One,
        BlendFactor::Zero,
        BlendOp::Add,
        SHARED_STATE
    };

    const BlendDesc BlendDesc::Addition = {
        true,
        BlendFactor::One,
        BlendFactor::Zero,
        BlendOp::Add,
        SHARED_STATE
    };

    const BlendDesc BlendDesc::AdditionAlpha = {
        true,
        BlendFactor::One,
        BlendFactor::Zero,
        BlendOp::Add,
        SHARED_STATE
    };

    const BlendDesc BlendDesc::Subtraction = {
        true,
        BlendFactor::One,
        BlendFactor::Zero,
        BlendOp::Add,
        SHARED_STATE
    };

}// namespace ob