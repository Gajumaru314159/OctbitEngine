//***********************************************************
//! @file
//! @brief		ウィンドウ
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Platform/TextInput.h>
#include <Framework/Platform/Window.h>

namespace ob::platform {

    String TextInput::GetText() {
        return TC("");
    }
    size_t TextInput::UpdateText(StringView text, size_t cursor, TextInputMode mode) {
        return 0;
    }
    size_t TextInput::UpdateText(StringView text, TextInputMode mode) {
        return 0;
    }
    String TextInput::GetEditingText() {
        return TC("");
    }


}// namespace ob::platform