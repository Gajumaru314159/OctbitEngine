//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#include <Framework/RHI/Types/BlendDesc.h>

namespace ob::rhi {

#define SHARED_STATE        \
        BlendFactor::One,   \
        BlendFactor::Zero,  \
        BlendOp::Add,       \
        ColorComponent::All


    const BlendDesc BlendDesc::None = {
        false,
        BlendFactor::One,
        BlendFactor::Zero,
        BlendOp::Add,
        SHARED_STATE
    };

    const BlendDesc BlendDesc::AlphaBlend = {
        true,
        BlendFactor::SrcAlpha,
        BlendFactor::OneMinusSrcAlpha,
        BlendOp::Add,
        BlendFactor::One,
        BlendFactor::OneMinusSrcAlpha,
        BlendOp::Add,
        ColorComponent::All,
    };

    const BlendDesc BlendDesc::Addition = {
        true,
        BlendFactor::SrcAlpha,
        BlendFactor::One,
        BlendOp::Add,
        BlendFactor::SrcAlpha,
        BlendFactor::One,
        BlendOp::Add,
        ColorComponent::All,
    };

    const BlendDesc BlendDesc::AdditionAlpha = {
        true,
        BlendFactor::SrcAlpha,
        BlendFactor::One,
        BlendOp::Add,
        BlendFactor::SrcAlpha,
        BlendFactor::One,
        BlendOp::Add,
        ColorComponent::All,
    };

    const BlendDesc BlendDesc::Subtraction = {
        true,
        BlendFactor::SrcAlpha,
        BlendFactor::One,
        BlendOp::Sub,
        BlendFactor::SrcAlpha,
        BlendFactor::One,
        BlendOp::Sub,
        ColorComponent::All,
    };

}