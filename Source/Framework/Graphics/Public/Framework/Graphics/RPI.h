//***********************************************************
//! @file
//! @brief		システム
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphics/Forward.h>
#include <Framework/Graphics/Render/RenderTask.h>

namespace ob::graphics {

    struct RPIDesc {
        struct ImageSystemDesc {
            u64 streamingImagePoolSize = 128 * 1024 * 1024;
            u64 attachmentImagePoolSize = 512 * 1024 * 1024;
        } image;

        struct GPUQuerySystemDesc {
            u32 occlusionQueryCount;
        } gpuQuery;

        struct DynamicDrawSystemDesc {
            u32 dynamicBufferPoolSize = 3 * 16 * 1024 * 1024;
        } dynamicDraw;
    };

    //@―---------------------------------------------------------------------------
    //! @brief      システムをServiceInjectorに登録
    //@―---------------------------------------------------------------------------
    class RPI {
    public:

        virtual void addRenderTask(RenderTask::RenderTaskDelegate func) = 0;

        //@―---------------------------------------------------------------------------
        //! @brief      ゲームループごとの更新を実行する
        //@―---------------------------------------------------------------------------
        virtual void simulate() = 0;

        //@―---------------------------------------------------------------------------
        //! @brief      描画ループの更新をする
        //@―---------------------------------------------------------------------------
        virtual void render() = 0;

        //@―---------------------------------------------------------------------------
        //! @brief      RenderFeatureを取得する
        //@―---------------------------------------------------------------------------
        virtual RenderFeature* findFeature(const TypeId&)const = 0;

        //@―---------------------------------------------------------------------------
        //! @brief      RenderFeatureを取得する(Template版)
        //@―---------------------------------------------------------------------------
        template<class T> T* findFeature()const { return reinterpret_cast<T*>(findFeature(TypeId::Get<T>())); }
                

    };

}