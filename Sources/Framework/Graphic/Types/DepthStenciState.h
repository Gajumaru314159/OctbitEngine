//***********************************************************
//! @file
//! @brief		デプス・ステンシル・ステート
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphic/Types/ComparisonFunc.h>
#include <Framework/Graphic/Types/StencilOp.h>

namespace ob::graphic {

    //@―---------------------------------------------------------------------------
    //! @brief  デプス・ステンシル・ステート
    //@―---------------------------------------------------------------------------
	struct DepthStencilState {
		struct {
			bool            enable{ true };
			bool            write{ true };
			ComparisonFunc  func{ ComparisonFunc::Less };
		} depth;

		struct {
			bool				enable{ false };
			u8		read_mask{ 0xff };
			u8		write_mask{ 0xff };
			StencilOp	failOp{ StencilOp::Keep };
			StencilOp	depthFailOp{ StencilOp::Keep };
			StencilOp	passOp{ StencilOp::Keep };
			ComparisonFunc		func{ ComparisonFunc::Always };
			u8		reference{ 0x00 };
		} stencil;
	};

}// namespcae ob::graphic