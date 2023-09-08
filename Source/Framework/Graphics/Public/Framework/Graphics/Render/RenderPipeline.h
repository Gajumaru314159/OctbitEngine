﻿//***********************************************************
//! @file
//! @brief		描画パイプライン
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphics/Forward.h>

#include <Framework/Graphics/Render/RenderStep.h>
#include <Framework/Graphics/Render/RenderView.h>

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
		//! @brief  コンストラクタ
		//@―---------------------------------------------------------------------------
		RenderPipeline(RenderPipelineDesc&& desc);
		~RenderPipeline();


		//@―---------------------------------------------------------------------------
		//! @brief  Viewを追加
		//@―---------------------------------------------------------------------------
		void createView(String name);

		//@―---------------------------------------------------------------------------
		//! @brief  描画処理
		//@―---------------------------------------------------------------------------
		void render();

	private:

		String m_name;

		Array<UPtr<RenderFeature>>	m_features;
		Array<UPtr<RenderView>>		m_views;
		Array<UPtr<RenderStep>>		m_steps;

		Ref<RenderContext> m_globalContext;

	};

}