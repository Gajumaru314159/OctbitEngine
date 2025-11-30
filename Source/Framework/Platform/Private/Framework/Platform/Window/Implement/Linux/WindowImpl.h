//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#pragma once
#ifdef OS_LINUX
#include "../IWindowImpl.h"

#include <X11/Xlib.h>
namespace ob::platform {
    struct NativeWindowHandle {
        ::Display* display = nullptr;
        ::Window window = 0;
    };
}

namespace ob::platform {

    class WindowImpl : public IWindowImpl {
    public:
        explicit WindowImpl(const WindowDesc& desc);
        ~WindowImpl() override;

        void show() override;
        void close() override;
        void maximize() override;
        void minimize() override;
        void moveToCenter() override;
        void restoreSize() override;

        bool isValid()const noexcept override;
        bool isMainWindow()const override;
        WindowStates getState()const override;

        Vec2 getScreenPoint(const Vec2& clientPoint)const override;
        Vec2 getClientPoint(const Vec2& screenPoint)const override;

        void setTitle(StringView title) override;
        const String& getTitle()const override;

        void setPosition(Vec2 position) override;
        Vec2 getPosition()const noexcept override;

        void setSize(Vec2 size) override;
        Vec2 getSize()const override;

        void setMode(WindowMode mode) override;
        WindowMode getMode()const override;

        WindowStyle getStyle()const override;
        void setStyle(WindowStyle style) override;

        void* getHandle()const override;
        String getTextInput() override;

        void addEventListener(WindowEventHandle& handle, WindowEventNotifier::delegate_type& func) override;

    private:
        void createWindow(const WindowDesc& desc);
        void destroyWindow();
        void updateState(WindowState state, bool enable);
        void applyFullscreen(bool enable);
        void updateWindowGeometryCache() const;

    private:
        NativeWindowHandle m_native{};
        WindowStates m_states;
        WindowMode m_mode = WindowMode::Window;
        WindowStyle m_style = WindowStyle::Sizable;
        mutable Vec2 m_position{0, 0};
        mutable Vec2 m_clientSize{0, 0};
        Vec2 m_restoreSize{1280, 720};
        String m_title;
        bool m_visible = false;
        bool m_closed = false;
        bool m_resizable = true;

        WindowEventNotifier m_notifier;
    };

}
#endif
