//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphics/Forward.h>

namespace ob::graphics {

    class RenderContext;
    class CameraData;

	//@―---------------------------------------------------------------------------
	//! @brief      描画パイプライン
    //! @details    描画呼び出しの基底となるクラスです。プログラム内で1つだけ存在します。
	//@―---------------------------------------------------------------------------
    class RenderStep {
    public:

        ~RenderStep() = default;

        virtual void execute(RenderContext& context, const CameraData& camera) = 0;

    };

}