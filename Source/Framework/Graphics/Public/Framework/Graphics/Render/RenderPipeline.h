//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphics/Forward.h>
#include <Framework/Graphics/Render/RenderScene.h>

namespace ob::graphics {

	//@―---------------------------------------------------------------------------
	//! @brief      描画ステップ
	//! @detailas   カメラごとに持ちます。
	//@―---------------------------------------------------------------------------
	class RenderPipeline {
	public:

		OB_RTTI();

		//@―---------------------------------------------------------------------------
		//! @brief      デストラクタ
		//@―---------------------------------------------------------------------------
		virtual ~RenderPipeline() = default;

		//@―---------------------------------------------------------------------------
		//! @brief      描画
		//@―---------------------------------------------------------------------------
		virtual void render(FG&,const Array<Ref<RenderView>>&) {};

	public:

		//@―---------------------------------------------------------------------------
		//! @brief      所属シーンを取得する
		//@―---------------------------------------------------------------------------
		RenderScene& getScene()const { return m_scene; }

		//@―---------------------------------------------------------------------------
		//! @brief      RenderFeatureを見つける
		//@―---------------------------------------------------------------------------
		template<class T> T* findFeature()const { return m_scene.findFeature<T>(); }
		RenderFeature* findFeature(TypeId typId)const { return m_scene.findFeature(typId); }

	protected:
		RenderPipeline(RenderScene& scene) :m_scene(scene) {}
	private:
		RenderScene& m_scene;
	};

}