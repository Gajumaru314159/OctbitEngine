//***********************************************************
//! @file
//! @brief		
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Utility/Ref.h>
#include <Framework/Graphics/Forward.h>
#include <Framework/Graphics/Camera.h>

namespace ob::graphics {

	//@―---------------------------------------------------------------------------
	//! @brief      描画機能
	//! @details    O3DEでいうところのFeatureProcessor。
	//!				初期状態は非アクティブです。
	//@―---------------------------------------------------------------------------
	class RenderTask {
	public:
		using RenderTaskDelegate = Delegate<void()>;
	public:
		RenderTask(RenderTaskDelegate delegate) 
			: m_delegate(delegate)
		{

		}
		void execute() {
			if (m_delegate) {
				m_delegate();
			}
		}
	private:
		RenderTaskDelegate m_delegate;
	};

}