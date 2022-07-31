﻿//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once

namespace ob::rendering {

	class IRenderer;

	//@―---------------------------------------------------------------------------
	//! @brief  説明
	//@―---------------------------------------------------------------------------
	class RenderSystem {
	public:

		//===============================================================
		// コンストラクタ / デストラクタ
		//===============================================================

		//@―---------------------------------------------------------------------------
		//! @brief  説明
		//@―---------------------------------------------------------------------------
		void setRenderer(UPtr<IRenderer>& renderer) {
			m_renderer = std::move(renderer);
		}

		void render() {
			if (!m_renderer)return;

			for (auto& camera : m_cameras) {
				m_renderer->render(m_context,camera);
			}
		}

	private:

		RenderContext m_context;
		UPtr<IRenderer> m_renderer;
		List<Camera> m_cameras;

	};






	//===============================================================
	// インライン関数
	//===============================================================
	//! @cond



	//! @endcond
}// namespcae ob