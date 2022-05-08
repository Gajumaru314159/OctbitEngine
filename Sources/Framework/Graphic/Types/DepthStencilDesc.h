//***********************************************************
//! @file
//! @brief		デプス・ステンシル定義
//! @author		Gajumaru
//***********************************************************
#pragma once

namespace ob::graphic {

#pragma region Enum

	//@―---------------------------------------------------------------------------
	//! @brief      ステンシル・オペレータ
	//@―---------------------------------------------------------------------------
	enum class StencilOp :u32 {
		Keep,				//!< 			
		Replace,			//!< 
		Zero,				//!< 
		Invert,				//!< 
		IncrementAndClamp,	//!< 
		DecrementAndClamp,	//!< 
		IncrementAndWrap,	//!< 
		DecrementAndWrap,	//!< 
	};


	//@―---------------------------------------------------------------------------
	//! @brief      比較関数 
	//@―---------------------------------------------------------------------------
	enum class ComparisonFunc :u32 {
		Never,			//!< 必ず失敗
		Always,			//!< 常に成功
		Equal,			//!< ソースとデストが等しい場合成功
		NotEqual,		//!< ソースがデストと異なる場合成功
		Less,			//!< ソースがデストより小さい場合成功
		Greater,		//!< ソースがデストより大きい場合成功
		LessEqual,		//!< ソースがデスト以下の場合成功
		GreaterEqual,	//!< ソースがデスト以上の場合成功
	};

#pragma endregion

    //@―---------------------------------------------------------------------------
    //! @brief  デプス・ステンシル定義
    //@―---------------------------------------------------------------------------
	struct DepthStencilDesc {
		struct {
			bool            enable{ false };
			bool            write{ true };
			ComparisonFunc  func{ ComparisonFunc::Less };
		} depth;

		struct {
			bool			enable{ false };
			u8				read_mask{ 0xff };
			u8				write_mask{ 0xff };
			StencilOp		failOp{ StencilOp::Keep };
			StencilOp		depthFailOp{ StencilOp::Keep };
			StencilOp		passOp{ StencilOp::Keep };
			ComparisonFunc	func{ ComparisonFunc::Always };
			u8				reference{ 0x00 };
		} stencil;
	};

}// namespcae ob::graphic