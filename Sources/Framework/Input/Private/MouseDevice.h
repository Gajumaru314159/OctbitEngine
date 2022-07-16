//***********************************************************
//! @file
//! @brief		マウスデバイス
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Input/Input.h>
#include <Framework/Core/Platform/WindowsHeaders.h>

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

        u32 getDeviceId()const override { return OB_FNV32("Mouse"); };

        void update();

        Vec2 position();
        bool down(MouseButton);
        
        //void bindButton(TriggerType,MouseButton, ButtonHandle&, const ButtonDelegate&);
    private:
        StaticArray<ButtonNotifier, enum_cast(MouseButton::MAX)> m_notifiers;
        Vec2 m_position;
        Vec2 m_deltaPos;
    };






    //===============================================================
    // インライン関数
    //===============================================================
    //! @cond



    //! @endcond
}// namespcae ob