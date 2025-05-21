//***********************************************************
//! @file
//! @brief		
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphics/Forward.h>

namespace ob::graphics {

	//! @brief RenderPass 情報
	struct RenderPassDesc {
		String				name;
		Func<RenderPass*()>	create;
	};

	//! @brief RenderPass 間の接続情報
	struct RenderPassConnection {
		String from;
		String to;
	};



	//! @brief RenderFeature 定義
	struct RenderFeatureDesc {
		Func<UPtr<RenderFeature>()>		create;
	};



	//! @brief RenderPipeline 定義
	struct RenderPipelineDesc {
		String							name;
		Vector<RenderPassConnection>	connections;
	};

	struct RenderFeatureInjector {
		Vector<Func<RenderFeature*(RenderScene&)>>	features;

		RenderFeatureInjector& add(Func<RenderFeature* (RenderScene&)> func) {
			features.emplace_back(func);
			return *this;
		}

		template<class T>
		RenderFeatureInjector& add() {
			features.emplace_back([](RenderScene& scene) { return new T(scene); });
			return *this;
		}
	};



	//! @brief RenderScene 定義
	struct RenderSceneDesc {
		String						name;
		RenderFeatureInjector		features;
		Vector<RenderPipelineDesc>	pipelines;
	};

}