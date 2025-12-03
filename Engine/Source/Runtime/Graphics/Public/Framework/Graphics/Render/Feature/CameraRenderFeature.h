//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphics/Render/RenderFeature.h>
#include <Framework/Graphics/Material/MaterialPropertiesDesc.h>

namespace ob::graphics {

	//! @brief      マテリアル描画機能
	class CameraRenderFeature : public RenderFeature {
	public:
		static MaterialPropertiesSetDesc GetProperties() {
			MaterialPropertiesSetDesc desc;
			desc.global.matrices = {
				"MatrixTest"
			};
			desc.view.matrices = {
				"MatrixV",
				"MatrixP",
				"MatrixVP",
				"MatrixInvV",
				"MatrixInvP",
				"MatrixInvVP",
			};
			desc.view.vectors = {
				"CameraPos",
				"ScreenSize",
				"CameraUp",
				"CameraRight",
				"CameraFront",
			};
			return desc;
		}
	public:

		OB_RTTI();

		CameraRenderFeature(RenderScene& scene) {

		}

	private:

	};

}