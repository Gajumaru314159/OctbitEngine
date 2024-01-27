//***********************************************************
//! @file
//! @brief		システム
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/Forward.h>
#include <Framework/Graphics/Forward.h>
#include <Framework/Core/Utility/Swapper.h>
#include <Framework/Core/Utility/HandleManager.h>

#include <Framework/Graphics/FrameGraph/FGResourcePool.h>

namespace ob::graphics {

    //@―---------------------------------------------------------------------------
    //! @brief      システムをServiceInjectorに登録
    //@―---------------------------------------------------------------------------
    void Register(ServiceInjector&);

    //@―---------------------------------------------------------------------------
    //! @brief      グラフィック
    //@―---------------------------------------------------------------------------
    class Graphics :public Singleton<Graphics> {
    public:

        //@―---------------------------------------------------------------------------
        //! @brief      コンストラクタ
        //@―---------------------------------------------------------------------------
        Graphics(rhi::RHI& rhi);

        //Ref<RenderScene> createScene();

        //@―---------------------------------------------------------------------------
        //! @brief      ゲームループごとの更新を実行する
        //@―---------------------------------------------------------------------------
        void update();

        void beginForParallel(s32 threadNum);
        void updateForParallel(s32 threadIndex);


        //@―---------------------------------------------------------------------------
        //! @brief      ネイティブの描画コマンドを発行して描画処理を開始する
        //@―---------------------------------------------------------------------------
        void execute();

        //@―---------------------------------------------------------------------------
        //! @brief      ネイティブの描画コマンドを発行して描画処理を開始する
        //@―---------------------------------------------------------------------------
        void wait();


        void addScene(Ref<RenderScene>&);
        void removeScene(Ref<RenderScene>&);
        

    private:

        rhi::RHI& m_rhi;

        UPtr<FG> m_fg{ nullptr };

        Swapper<Ref<rhi::CommandList>> m_commandLists;

        Array<Ref<RenderScene>> m_scenes;

        FGResourcePool m_fgResourcePool;

        HandleManager<RenderScene> m_renderSceneManager;

    };

}