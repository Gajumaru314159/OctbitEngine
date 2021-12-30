//***********************************************************
//! @file
//! @brief		システム・イベント型
//! @author		Gajumaru
//***********************************************************
#pragma once

namespace ob {
    namespace platform {


        //@―---------------------------------------------------------------------------
        //! @brief  ウィンドウ・イベント・タイプ
        //@―---------------------------------------------------------------------------
        enum class WindowEventType {
            Move,
            Size,
            Activate,
            Deactivate,
            Focus,
            Close,
            Destroy,
            Minimize,
            Maximize,
        };
        

        //@―---------------------------------------------------------------------------
        //! @brief  システム・イベント・タイプ
        //@―---------------------------------------------------------------------------
        enum class SystemEventType {
            Unknown,

            Quit,
            Close,
            WindowActivate,
            WindowsDeactivate,

            MouseDown,
            MouseUp,
            MouseMove,
            MouseWheel,

            KeyDown,
            KeyUp,
            KeyChar,

            WindowSizeChanged,

            DragEnter,
            DragDrop,
        };

        enum class MouseButton {
            None,
            Left,
            Right,
            Middle,
            X1,
            X2,
        };

        //@―---------------------------------------------------------------------------
        //! @brief  ウィンドウ・イベント
        //@―---------------------------------------------------------------------------
        using WindowEvent = delegate<void(WindowEventType)>;


        //@―---------------------------------------------------------------------------
        //! @brief  説明
        //@―---------------------------------------------------------------------------
        using MouseMoveEvent = delegate<void(void)>;

    }// namespace platform
}// namespcae ob