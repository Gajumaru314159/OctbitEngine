//***********************************************************
//! @file
//! @brief		システム
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphics/Forward.h>

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

        virtual void addScene(Ref<RenderScene>) = 0;
        virtual void removeScene(Ref<RenderScene>) = 0;
        virtual Ref<RenderScene> findScene(StringView name)const = 0;

        //@―---------------------------------------------------------------------------
        //! @brief      ゲームループごとの更新を実行する
        //@―---------------------------------------------------------------------------
        virtual void simulate() = 0;

        //@―---------------------------------------------------------------------------
        //! @brief      描画ループの更新をする
        //@―---------------------------------------------------------------------------
        virtual void render() = 0;
    };

}