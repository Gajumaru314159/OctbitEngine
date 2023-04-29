﻿//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Utility/Ref.h>
#include <Framework/Graphics/Forward.h>

namespace ob::graphics {

	//@―---------------------------------------------------------------------------
	//! @brief      描画パイプライン
    //! @details    描画呼び出しの基底となるクラスです。プログラム内で1つだけ存在します。
	//@―---------------------------------------------------------------------------
	class RenderPipeline {
	public:

		//@―---------------------------------------------------------------------------
		//! @brief  描画処理
		//@―---------------------------------------------------------------------------
        virtual void render(RenderContext& context, Span<const Ref<Camera>> cameras) {}

        virtual void onCameraAdded(Ref<Camera>) {}
        virtual void onCameraRemoved(Ref<Camera>) {}

	};

}