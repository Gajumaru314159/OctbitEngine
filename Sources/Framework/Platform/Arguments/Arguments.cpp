//***********************************************************
//! @file
//! @brief		コマンドライン引数
//! @author		Gajumaru
//***********************************************************
#pragma once
#include "Arguments.h"
#include <Framework/Foundation/String/StringEncoder.h>


#ifdef OS_WINDOWS

#include <Windows.h>
static void GetCommandLineForWindows(ob::platform::Arguments::container_type& dest) {
    dest.clear();
    int numArgs;
    LPWSTR* ppArgs = ::CommandLineToArgvW(GetCommandLineW(), &numArgs);
    for (int i = 0; i < numArgs; ++i) {
        ob::StringBase<wchar_t> warg = ppArgs[i];
        ob::String arg;
        ob::StringEncoder::Encode(warg, arg);
        dest.push_back(arg);
    }
}

#elif


#endif



namespace ob::platform {

    //@―---------------------------------------------------------------------------
    //! @brief  コンストラクタ
    //@―---------------------------------------------------------------------------
    Arguments::Arguments() {
        ::GetCommandLineForWindows(m_args);
    }

    const String& Arguments::at(s32 index)const {
        return m_args.at(index);
    }


    Arguments::const_iterator Arguments::cbegin()const noexcept {
        return m_args.cbegin();
    }

    Arguments::const_iterator Arguments::cend()const noexcept {
        return m_args.cend();
    }

    s32 Arguments::size()const noexcept {
        return static_cast<s32>(m_args.size());
    }

    //@―---------------------------------------------------------------------------
    //! @brief          空か
    //@―---------------------------------------------------------------------------
    bool Arguments::empty()const noexcept {
        return m_args.empty();
    }


}// namespace ob::platform


