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

        virtual void    show() = 0;
        virtual void    close() = 0;
        virtual void    addEventListener(WindowEventType type, const WindowEvent& event) = 0;
        virtual void    setTitle(StringView title) = 0;
        virtual auto    title() const noexcept -> const String& = 0;
        virtual Size    size()const = 0;
        virtual Point   getScreenPoint(const Point& clientPoint)const = 0;
        virtual Point   getClientPoint(const Point& screenPoint)const = 0;
        virtual bool    isValid()const noexcept = 0;
    };

}// namespace ob::platform