//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Input/InputType.h>
#include <Framework/Input/Input.h>

namespace ob::input {

    //@―---------------------------------------------------------------------------
    //! @brief  説明
    //@―---------------------------------------------------------------------------
    class InputManager:public Singleton<InputManager> {
    public:

        //===============================================================
        // コンストラクタ / デストラクタ
        //===============================================================
        InputManager();
        ~InputManager();

        //@―---------------------------------------------------------------------------
        //! @brief  説明
        //@―---------------------------------------------------------------------------
        void update();

        IInputDevice* findDevice(u32 guid,u32 user);

    public:

        Pimpl<class KeyboardDevice> m_impl;
        Pimpl<class MouseImpl> m_mouse;

    };

}// namespcae ob