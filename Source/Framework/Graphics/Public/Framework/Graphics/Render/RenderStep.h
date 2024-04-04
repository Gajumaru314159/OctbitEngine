//***********************************************************
//! @file
//! @brief		
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/Forward.h>
#include <Framework/Graphics/Forward.h>

namespace ob::graphics {

	//@―---------------------------------------------------------------------------
	//! @brief      RenderView毎に持つ
	//@―---------------------------------------------------------------------------
	class RenderStep {
	public:
		OB_RTTI();
		virtual ~RenderStep() = default;
	protected:
		RenderStep(RenderView& view) : m_view(view) {}
	protected:
		RenderView& m_view;
	};


	//@―---------------------------------------------------------------------------
	//! @brief      RenderFeatureのファクトリ
	//@―---------------------------------------------------------------------------
	class RenderStepInjector {
	public:
		using CreateFunc = Func<RenderStep* (RenderView&)>;
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
		template<class T,class = std::enable_if_t<std::is_base_of<RenderStep,T>::value>>
		void add() {
			add([](RenderView& scene) {return new T(scene); });
		}

	private:

		Array<UPtr<RenderStep>> create(RenderView& view)const {
			Array<UPtr<RenderStep>> features;
			for (auto& creator : m_creators) {
				if (auto step = creator(view)) {
					features.emplace_back(step);
				}
			}
			return std::move(features);
		}

	protected:
		friend class RenderView;
		Array<CreateFunc> m_creators;
	};


}