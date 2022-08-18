//***********************************************************
//! @file
//! @brief		マウス・デバイス
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Input/Interface/IInputDevice.h>
#include <Framework/Core/Platform/WindowsHeaders.h>
#ifdef OS_WINDOWS
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#endif
namespace ob::input {

    //@―---------------------------------------------------------------------------
    //! @brief  マウス・デバイス
    //@―---------------------------------------------------------------------------
    class DirectInputDevice:public IInputDevice {
    public:

        //@―---------------------------------------------------------------------------
        //! @brief  コンストラクタ
        //@―---------------------------------------------------------------------------
        DirectInputDevice();

        //@―---------------------------------------------------------------------------
        //! @brief  デストラクタ
        //@―---------------------------------------------------------------------------
        ~DirectInputDevice();

        //@―---------------------------------------------------------------------------
        //! @brief  デバイスID
        //@―---------------------------------------------------------------------------
        u32 getDeviceId()const override { return OB_FNV32("DirectInput"); };

        //@―---------------------------------------------------------------------------
        //! @brief  更新
        //@―---------------------------------------------------------------------------
        void update()override;

        //@―---------------------------------------------------------------------------
        //! @brief  ボタンの入力状態を取得
        //@―---------------------------------------------------------------------------
        ButtonStates getButtonStates(u32 code)const override;

        //@―---------------------------------------------------------------------------
        //! @brief  軸の入力状態を取得
        //@―---------------------------------------------------------------------------
        f32 getAxisValue(u32 code)const override;

        //@―---------------------------------------------------------------------------
        //! @brief  ボタン入力イベントをバインド
        //@―---------------------------------------------------------------------------
        bool bindButton(u32 code, ButtonState state, ButtonHandle& handle, const ButtonDelegate& func) override;

        //@―---------------------------------------------------------------------------
        //! @brief  ボタン入力イベントをバインド
        //@―---------------------------------------------------------------------------
        bool bindAxis(u32 code, AxisHandle& handle, const AxisDelegate& func) override;

    private:

        struct KeyState {
            HashMap<ButtonState, ButtonNotifier> notifiers;
            ButtonStates prev;
            ButtonStates next;
        };
        struct AxisState {
            AxisNotifier notifier;
            f32 prev{0.0f};
            f32 next{0.0f};
        };

        DIJOYSTATE m_mouseState;
        LPDIRECTINPUT8 m_interface;
        LPDIRECTINPUTDEVICE8 m_mouse;

        HashMap<MouseButton, KeyState> m_states;
        HashMap<MouseAxis,AxisState> m_axisStates;

        Vec2 m_position;
        Vec2 m_deltaPos;

    };






    //===============================================================
    // インライン関数
    //===============================================================
    //! @cond



    //! @endcond
}// namespcae ob