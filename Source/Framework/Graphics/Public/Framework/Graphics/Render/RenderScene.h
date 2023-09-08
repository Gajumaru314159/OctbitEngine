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

	enum class RenderPipelineId{};

	//@―---------------------------------------------------------------------------
	//! @brief      描画シーン
	//! @details	GraphicsにおけるWorld。
	//@―---------------------------------------------------------------------------
	class RenderScene final : public RefObject{
	public:

		RenderScene(StringView name);

		StringView getName()const { return ""; }

		template<class T,class... TArgs>
		RenderPipelineId createRenderPipeline(TArgs&&... args);
		
		void simulate();
		void render();


	public:
		
		static Ref<RenderScene> Create(StringView name);
		

	private:

		RenderPipelineId addRenderPipeline(UPtr<RenderPipeline>);


	private:
		String m_name;
		HashMap<RenderPipelineId,UPtr<RenderPipeline>> m_pipelines;

	};


	template<class T, class... TArgs>
	RenderPipelineId RenderScene::createRenderPipeline(TArgs&&... args) {
		std::make_unique<T>(std::move(args)...);
		return RenderPipelineId{ 0 };// addRenderPipeline(std::make_unique<T>(args...));
	}


}