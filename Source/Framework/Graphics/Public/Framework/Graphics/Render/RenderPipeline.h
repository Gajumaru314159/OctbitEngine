//***********************************************************
//! @file
//! @brief		描画パイプライン
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Utility/Ref.h>
#include <Framework/Graphics/Forward.h>
#include <Framework/Graphics/Camera.h>

namespace ob::graphics {

	//@―---------------------------------------------------------------------------
	//! @brief      描画パイプライン
	//! @details    描画呼び出しの基底となるクラスです。プログラム内で1つだけ存在します。
	//!				Unityでは
	//!				* ビルトイン
	//!				* UniversalRenderPipeline
	//!				* HeighDefinitionRenderPipeline
	//!				などに当たります。
	//@―---------------------------------------------------------------------------
	class RenderPipeline {
	public:

		//@―---------------------------------------------------------------------------
		//! @brief  描画処理
		//@―---------------------------------------------------------------------------
		virtual void render(RenderContext& context, Span<CameraData> cameras) {}

	};

}