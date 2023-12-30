//***********************************************************
//! @file
//! @brief		
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphics/Forward.h>
#include <Framework/Core/Utility/Ref.h>
#include <Framework/Core/Utility/DI.h>
#include <Framework/Graphics/Render/RenderFeature.h>

namespace ob::graphics {

	//@―---------------------------------------------------------------------------
	//! @brief      RenderFeatureのファクトリ
	//@―---------------------------------------------------------------------------
	class RenderFeatureInjector {
	public:
		using CreateFunc = Func<RenderFeature* (RenderScene&)>;
	public:

		//@―---------------------------------------------------------------------------
		//! @brief      RenderFeatureを追加する
		//@―---------------------------------------------------------------------------
		void add(CreateFunc&& creator) {
			m_creators.push_back(creator);
		}

		//@―---------------------------------------------------------------------------
		//! @brief      RenderFeatureを追加する
		//@―---------------------------------------------------------------------------
		template<class T>
		void add() {
			add([](RenderScene& scene) {return new T(scene); });
		}

	private:

		Array<UPtr<RenderFeature>>&& create(RenderScene& scene)const {
			Array<UPtr<RenderFeature>> features;
			for (auto& creator : m_creators) {
				if (auto feature = creator(scene)) {
					features.emplace_back(feature);
				}
			}
			return std::move(features);
		}

	private:
		friend class RenderScene;
		Array<CreateFunc> m_creators;
	};

	//@―---------------------------------------------------------------------------
	//! @brief      RenderPipelineのファクトリ
	//@―---------------------------------------------------------------------------
	class RenderPipelineInjector {
	public:
		using CreateFunc = Func<RenderPipeline* (RenderScene&)>;
	public:

		//@―---------------------------------------------------------------------------
		//! @brief      RenderPipelineを追加する
		//@―---------------------------------------------------------------------------
		void add(CreateFunc&& creator) {
			m_creators.push_back(creator);
		}

		//@―---------------------------------------------------------------------------
		//! @brief      RenderPipelineを追加する
		//@―---------------------------------------------------------------------------
		template<class T>
		void add() {
			add([](RenderScene& scene) {return new T(scene); });
		}

	private:

		Array<UPtr<RenderPipeline>>&& create(RenderScene& scene)const {
			Array<UPtr<RenderPipeline>> features;
			for (auto& creator : m_creators) {
				if (auto feature = creator(scene)) {
					features.emplace_back(feature);
				}
			}
			return std::move(features);
		}

	private:
		friend class RenderScene;
		Array<CreateFunc> m_creators;
	};

	//@―---------------------------------------------------------------------------
	//! @brief      RenderScene定義
	//@―---------------------------------------------------------------------------
	struct RenderSceneDesc {
		String					name;
		RenderFeatureInjector	features;
		RenderPipelineInjector	pipelines;
	};


}