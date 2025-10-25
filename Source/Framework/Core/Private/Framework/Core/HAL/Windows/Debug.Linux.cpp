//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/HAL/Platform.h>
#include <string>
#include <format>
#ifdef OS_LINUX
#include <iostream>

//! @brief  システムログ出力 
void OutputDebugLog(const char* message) {
    // std::cout << message << std::endl;
}

//! @brief  ブレークポイントを呼び出し
void ShowMessageBox(const char* message) {
    std::string cmd = std::format(R"(zenity --info --title="OctbitEngine" --no-wrap --text="{}")",message);
    std::system(cmd.c_str());
}

#endif