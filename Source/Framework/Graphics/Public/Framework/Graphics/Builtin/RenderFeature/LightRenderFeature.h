//***********************************************************
//! @file
//! @brief		
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphics/Render/RenderFeature.h>
#include <Framework/Graphics/Builtin/RenderFeature/MaterialRenderFeature.h>
#include <Framework/Graphics/Material/MaterialPropertiesDesc.h>

namespace ob::graphics {

	class MaterialRenderFeature;

	//! @brief      マテリアル描画機能
	class LightRenderFeature : public RenderFeature {
	public:

		static MaterialPropertiesSetDesc GetProperties() {
			MaterialPropertiesSetDesc desc;
			desc.scene.buffers = {
				// "SpotLightBuffer",
				// "PointLightBuffer",
				// "DirectionalLightBuffer",
			};
			desc.scene.vectors = {
				"LightDir"
			};
			return desc;
		}
	public:

		OB_RTTI();

		LightRenderFeature(MaterialRenderFeature& materialRF)
		{
		}

	};
}