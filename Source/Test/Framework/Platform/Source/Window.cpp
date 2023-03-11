//***********************************************************
//! @file
//! @brief		Box のテスト
//! @author		Gajumaru
//***********************************************************
#include <Framework/Platform/Window.h>

using namespace ob;

TEST(Window, ShowHide) {

    using namespace ob::platform;
    WindowDesc desc;
    Window window = Window(desc);

    window.show();
    window.moveToCenter();

    window.maximize();
    ASSERT_TRUE(window.getState().has(WindowState::Maximized));

    window.minimize();
    ASSERT_TRUE(window.getState().has(WindowState::Minimized));

    window.restoreSize();

}

TEST(Window, Title) {

    using namespace ob::platform;
    WindowDesc desc;
    desc.title = TC("AAA");
    Window window = Window(desc);
    ASSERT_EQ(window.getTitle(), TC("AAA"));

    window.setTitle(TC("BBB"));
    ASSERT_EQ(window.getTitle(), TC("BBB"));

}

TEST(Window, PosSize) {

    using namespace ob::platform;
    WindowDesc desc;
    desc.clientSize = { 512, 256 };
    Window window = Window(desc);
    ASSERT_EQ(window.getSize(), Vec2(512,256));

    window.setSize({ 800, 600 });
    ASSERT_EQ(window.getSize(), Vec2(800,600));


    window.setPosition({ 123,250 });
    ASSERT_EQ(window.getPosition(), Vec2(123, 250));

    window.setPosition({ 456,250 });
    ASSERT_EQ(window.getPosition(), Vec2(456, 250));

}