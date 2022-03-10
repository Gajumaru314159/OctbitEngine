//***********************************************************
//! @file
//! @brief		ウィンドウ・インターフェース
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Platform/Type/SystemEventType.h>
#include <Framework/Platform/Type/CursorType.h>
#include <Framework/Platform/Type/WindowCreationParams.h>
#include <Framework/Core/Type/Point.h>
#include <Framework/Core/Type/Size.h>

namespace ob::platform {

    //@―---------------------------------------------------------------------------
    //! @brief  ウィンドウ・インターフェース
    //@―---------------------------------------------------------------------------
    class IWindowImpl {
    public:
        virtual ~IWindowImpl() {};

        virtual void    Show() = 0;
        virtual void    Close() = 0;
        virtual void    AddEventListener(WindowEventType type, const WindowEvent& event) = 0;
        virtual void    SetWindowTitle(StringView title) = 0;
        virtual auto    GetWindowTitle() const noexcept -> const String& = 0;
        virtual Size    GetSize()const = 0;
        virtual Point   GetScreenPoint(const Point& clientPoint)const = 0;
        virtual Point   GetClientPoint(const Point& screenPoint)const = 0;
        virtual bool    isValid()const noexcept = 0;
    };

}// namespace ob::platform