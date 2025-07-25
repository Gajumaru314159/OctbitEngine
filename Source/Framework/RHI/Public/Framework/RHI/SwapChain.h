//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/GraphicObject.h>
#include <Framework/RHI/Types/SwapChainDesc.h>

namespace ob::rhi {

    //! @brief  スワップチェーン・イベント
    OB_EVENT_NOTIFIER(SwapChainEvent);


    //! @brief      スワップチェーン
    //! 
    //! @details    モニターに表示するバッファを複数のバッファから切り替えることで
    //!             表示のちらつきを解決する。
    class SwapChain :public GraphicObject {
    public:

        //! @brief      生成
        static Ref<SwapChain> Create(const SwapChainDesc& desc);

    public:

        //! @brief      定義を取得
        virtual const SwapChainDesc& getDesc()const noexcept = 0;


        //! @brief      更新
        virtual void update() = 0;


        //! @brief      イベントリスナ追加
        virtual void addEventListener(SwapChainEventHandle& handle, SwapChainEventDelegate func) = 0;

    };


}