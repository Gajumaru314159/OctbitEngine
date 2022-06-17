//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Input/InputType.h>
#include <Framework/Core/Platform/WindowsHeaders.h>

namespace ob::input {

    //@―---------------------------------------------------------------------------
    //! @brief  説明
    //@―---------------------------------------------------------------------------
    class MouseImpl {
    public:
        using ButtonNotifier  = EventNotifier<>;
        using ButtonHandle    = ButtonNotifier::Handle;
        using ButtonDelegate  = ButtonNotifier::delegate_type;
    public:

        MouseImpl();

        void update();

        Vec2 position();
        bool down(MouseButton);

        void bindButton(TriggerType,MouseButton, ButtonHandle&, const ButtonDelegate&);
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