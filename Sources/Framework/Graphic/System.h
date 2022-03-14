﻿//***********************************************************
//! @file
//! @brief		グラフィック・システム
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphic/Types/GraphicAPI.h>
#include <Framework/Graphic/Interface/IDevice.h>
#include <Framework/Graphic/Device.h>

namespace ob::graphic {
    class GraphicObject;
}

namespace ob::graphic {

    //@―---------------------------------------------------------------------------
    //! @brief  説明
    //@―---------------------------------------------------------------------------
    class System :public Singleton<System> {
    public:

        //===============================================================
        // コンストラクタ / デストラクタ
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief      コンストラクタ
        //! 
        //! @details    プラットフォームにより使用できないAPIがあります。
        //!             使用できるプラットフォームは GraphicAPI を参照してください。
        //! @param api  使用するグラフィックAPIタイプ
        //@―---------------------------------------------------------------------------
        System(GraphicAPI api);


        //@―---------------------------------------------------------------------------
        //! @brief      デストラクタ
        //@―---------------------------------------------------------------------------
        ~System();


        //@―---------------------------------------------------------------------------
        //! @brief      デバイスを取得デバイスを取得
        //@―---------------------------------------------------------------------------
        Device& getDevice();


        //@―---------------------------------------------------------------------------
        //! @brief      デバイスを取得デバイスを取得
        //@―---------------------------------------------------------------------------
        void requestRelease(GraphicObject* pObject);


    private:

        Device m_device;
        std::vector<ob::stack<GraphicObject*>> m_delayReleaseStack;
        s32 m_nowStackIndex;

    };

}// namespcae ob::graphic