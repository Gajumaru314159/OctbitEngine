//***********************************************************
//! @file
//! @brief		ブレンド定義
//! @author		Gajumaru
//***********************************************************
#include <Framework/Graphic/Types/BlendDesc.h>

namespace ob::graphic {

#define SHARED_STATE        \
        BlendFactor::One,   \
        BlendFactor::Zero,  \
        BlendOp::Add,       \
        false,              \
        LogicOp::Noop,      \
        3


    const BlendDesc BlendDesc::None = {
        false,
        BlendFactor::One,
        BlendFactor::Zero,
        BlendOp::Add,
        SHARED_STATE
    };

    const BlendDesc BlendDesc::AlphaBlend = {
        false,
        BlendFactor::One,
        BlendFactor::Zero,
        BlendOp::Add,
        SHARED_STATE
    };

    const BlendDesc BlendDesc::Addition = {
        false,
        BlendFactor::One,
        BlendFactor::Zero,
        BlendOp::Add,
        SHARED_STATE
    };

    const BlendDesc BlendDesc::AdditionAlpha = {
        false,
        BlendFactor::One,
        BlendFactor::Zero,
        BlendOp::Add,
        SHARED_STATE
    };

    const BlendDesc BlendDesc::Subtraction = {
        false,
        BlendFactor::One,
        BlendFactor::Zero,
        BlendOp::Add,
        SHARED_STATE
    };

#undef SHARED_STATE

}// namespace ob