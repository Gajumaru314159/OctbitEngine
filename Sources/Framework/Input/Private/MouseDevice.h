//***********************************************************
//! @file
//! @brief		マウスデバイス
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Input/Input.h>
#include <Framework/Core/Platform/WindowsHeaders.h>
#ifdef OS_WINDOWS
#include <dinput.h>
#endif
namespace ob::input {


    //@―---------------------------------------------------------------------------
    //! @brief  説明
    //@―---------------------------------------------------------------------------
    class MouseDevice:public IInputDevice {
    public:
        using ButtonNotifier  = EventNotifier<>;
        using ButtonHandle    = ButtonNotifier::Handle;
        using ButtonDelegate  = ButtonNotifier::delegate_type;
    public:

        MouseDevice();
        ~MouseDevice();

        u32 getDeviceId()const override { return OB_FNV32("Mouse"); };

        void update()override;

        InputStates getInputStates(u32 code)const override{ return {}; }
        f32  getF32(u32 code) const override{ return 0.0f; }
        Vec2 getVec2(u32 code) const override{ return { 0.0f,0.0f }; }
        Vec3 getVec3(u32 code) const override{ return { 0.0f,0.0f,0.0f }; }


        Vec2 position();
        bool down(MouseButton);
        
        //void bindButton(TriggerType,MouseButton, ButtonHandle&, const ButtonDelegate&);
    private:

        struct State {
            InputNotifier notifier;
            InputStates states;
            InputStates prevStates;
        };

        LPDIRECTINPUT8 m_interface;
        LPDIRECTINPUTDEVICE8 m_mouse;
        DIMOUSESTATE m_state;


        StaticArray<State, enum_cast(MouseButton::MAX)> m_states;
        Vec2 m_position;
        Vec2 m_deltaPos;
    };






    //===============================================================
    // インライン関数
    //===============================================================
    //! @cond



    //! @endcond
}// namespcae ob