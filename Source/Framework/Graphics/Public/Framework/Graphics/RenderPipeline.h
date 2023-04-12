//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once

namespace ob::graphics {

	class RenderContext;
	class Camera;

	//@―---------------------------------------------------------------------------
	//! @brief      描画パイプライン
    //! @details    描画呼び出しの基底となるクラスです。プログラム内で1つだけ存在します。
	//@―---------------------------------------------------------------------------
	class RenderPipeline {
	public:

		//@―---------------------------------------------------------------------------
		//! @brief  描画処理
		//@―---------------------------------------------------------------------------
        virtual void render(RenderContext& context, Span<const Ref<Camera>> cameras) {

            // RederPipelineManager::Get().callBeginCameraRenderingEvent();
            // CameraRendering  カメラごと
            // ContextRendering renderの開始

        }

        virtual void onCameraAdded(Ref<Camera>) {

        }
        virtual void onCameraRemoved(Ref<Camera>) {

        }

	};

    // https://docs.unity3d.com/Packages/com.unity.render-pipelines.universal@7.1/api/UnityEngine.Rendering.Universal.ScriptableRenderPass.html
    class RenderFeature {
    public:

        //@―---------------------------------------------------------------------------
        //! @brief      描画処理
        //! @details    
        //@―---------------------------------------------------------------------------
        virtual void render(RenderContext& context, const Ref<Camera>& camera) {

        }

    };


    class RenderFeatureGroup {
    public:

        RenderFeatureGroup(RenderPipeline&,Ref<Camera> camera) {

        }

        const Ref<Camera>& getCamera()const;

        Ref<rhi::RenderTexture> addRenderTexture(rhi::TextureFormat format, Vec3 scaleMultiply) {

        }

        Ref<rhi::RenderTexture> findRenderTexture(engine::Name name) {
            auto found = m_renderTextureMap.find(name);
            if (found == m_renderTextureMap.end())return nullptr;
            return found->second;
        }

    private:

        Ref<Camera> m_camera;
        HashMap<engine::Name, Ref<rhi::RenderTexture>> m_renderTextureMap;

    };





    class RenderFeatureUtil {
    public:

        //@―---------------------------------------------------------------------------
        //! @brief      マテリアルを使用してCommandBufferに入力テクスチャを出力テクスチャにコピーします。
        //@―---------------------------------------------------------------------------
        static void blit(Ref<CommandBuffer>& cmd, Ref<rhi::Texture> src, Ref<rhi::RenderTexture> dst, Ref<Material> material, engine::Name pass) {
            // TODO setTexture()では対応できないのでPropertyBlockを用意する必要がある
        }

    };


	class UniversalRenderPipeline : public RenderPipeline {
	public:

		void render(RenderContext& context, Span<const Ref<Camera>> cameras) override{

            for(auto& camera : cameras)
            {
                Array<Ref<RenderFeature>> features;// = camera.getRenderFeatureList();
                
                for (auto& feature : features) {
                    feature->render(context,camera);
                }
            }
		}

    private:



	};

}