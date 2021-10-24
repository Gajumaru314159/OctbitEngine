//***********************************************************
//! @file
//! @brief		ウィンドウ・インターフェース
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Runtime/Platform/Type/SystemEventType.h>
#include <Runtime/Platform/Type/CursorType.h>
#include <Runtime/Platform/Type/GeometoryType.h>
#include <Runtime/Platform/Type/WindowCreationParams.h>

namespace ob {
    namespace platform {

        //@―---------------------------------------------------------------------------
        //! @brief  ウィンドウ・インターフェース
        //@―---------------------------------------------------------------------------
        class IWindowImpl {
        public:
            virtual ~IWindowImpl() {};

            virtual void Show() = 0;
            virtual void Close() = 0;
            virtual void AddEventListener(WindowEventType type, const WindowEvent& event)=0;
            virtual void SetWindowTitle(const String& title) = 0;
            virtual Size GetSize() = 0;
            virtual Point GetScreenPoint(const Point& clientPoint)const = 0;
            virtual Point GetClientPoint(const Point& screenPoint)const = 0;
            virtual bool IsValid()const noexcept = 0;
        };

    }// namespace platform
}// namespcae ob