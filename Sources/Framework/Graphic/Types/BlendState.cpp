//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include <Framework/Graphic/Types/BlendState.h>

namespace ob::graphic {

#define SHARED_STATE        \
        BlendFactor::One,   \
        BlendFactor::Zero,  \
        BlendOp::Add,       \
        false,              \
        LogicOp::Noop,      \
        3


    const BlendState BlendState::None = {
        false,
        BlendFactor::One,
        BlendFactor::Zero,
        BlendOp::Add,
        SHARED_STATE
    };

    const BlendState BlendState::AlphaBlend = {
        false,
        BlendFactor::One,
        BlendFactor::Zero,
        BlendOp::Add,
        SHARED_STATE
    };

    const BlendState BlendState::Addition = {
        false,
        BlendFactor::One,
        BlendFactor::Zero,
        BlendOp::Add,
        SHARED_STATE
    };

    const BlendState BlendState::AdditionAlpha = {
        false,
        BlendFactor::One,
        BlendFactor::Zero,
        BlendOp::Add,
        SHARED_STATE
    };

    const BlendState BlendState::Subtraction = {
        false,
        BlendFactor::One,
        BlendFactor::Zero,
        BlendOp::Add,
        SHARED_STATE
    };

#undef SHARED_STATE

}// namespace ob