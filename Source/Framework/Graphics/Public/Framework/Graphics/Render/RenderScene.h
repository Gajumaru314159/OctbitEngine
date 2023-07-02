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

	/*
	
	DrawPacket
	MeshDrawPacket
	ModelDataInstance
	
	RenderScene
		RenderPipeline
		RenderItem
	

	class RenderFeature {

	};
	struct MeshObject {
		Transform	transform;
		Bound		bound;
		SortKey		sortkey;

	};

	class MeshRenderFeature : public RenderFeature{
	public:
		virtual Ref<Mesh> acquireMesh() = 0;
	};


	*/

	struct RenderSceneDesc {
		String			name;
		Array<String>	features;
	};


	//@―---------------------------------------------------------------------------
	//! @brief      描画シーン
	//! @details	GraphicsにおけるWorld。
	//@―---------------------------------------------------------------------------
	class RenderScene : public RefObject{
	public:

		static Ref<RenderScene> Create(const RenderSceneDesc&);

	private:

		RenderScene(const RenderSceneDesc&);

		template<class T, class... Args>
		void setRenderPipeline(Args&&... args) {
			m_renderPipeline = std::make_unique<T>(args...);
		}

		void upadte();

	private:

		UPtr<RenderPipeline> m_renderPipeline;
		RenderSceneDesc m_desc;

		//Array <Ref<RenderFeature>> m_features;


	};

}