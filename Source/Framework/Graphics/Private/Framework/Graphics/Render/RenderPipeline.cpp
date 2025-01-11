//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include <Framework/Graphics/Forward.h>
#include <Framework/Graphics/Render/RenderScene.h>

namespace ob::graphics {

	//! @brief      描画ステップ
	//! @detailas   カメラごとに持ちます。
	class RenderPipeline {
	public:

		OB_RTTI();

		//! @brief      デストラクタ
		virtual ~RenderPipeline() = default;

		//! @brief      描画
		virtual void render(FG&,const Vector<Ref<RenderView>>&) {};

	public:

		//! @brief      所属シーンを取得する
		RenderScene& getScene()const { return m_scene; }

		//! @brief      RenderFeatureを見つける
		template<class T> T* findFeature()const { return findFeature(Type::Get<T>()); }
		RenderFeature* findFeature(Type typId)const;

	protected:
		RenderPipeline(RenderScene& scene) :m_scene(scene) {}
	private:
		RenderScene& m_scene;
	};

}