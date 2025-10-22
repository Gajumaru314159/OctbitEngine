//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#include <Framework/Platform/Arguments.h>
#include <Framework/Core/String/StringEncoder.h>

#ifdef OS_WINDOWS

#include <Windows.h>
static void GetCommandLine(ob::platform::Arguments::container_type& dest) {
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

#elif defined(OS_LINUX)
#include <wordexp.h>
static void GetCommandLine(ob::platform::Arguments::container_type& dest) {
    dest.clear();
    LOG_ERROR("GetCommandLineは未実装です");
}

#endif



namespace ob::platform {

    //! @brief  コンストラクタ
    Arguments::Arguments() {
        ::GetCommandLine(m_args);
    }


    //! @brief  引数取得
    const String& Arguments::at(s32 index)const {
        return m_args.at(index);
    }


    //! @brief  開始イテレータ(const)
    Arguments::const_iterator Arguments::begin()const noexcept {
        return m_args.cbegin();
    }


    //! @brief  終了イテレータ(const)
    Arguments::const_iterator Arguments::end()const noexcept {
        return m_args.cend();
    }


    //! @brief  引数サイズ
    s32 Arguments::size()const noexcept {
        return static_cast<s32>(m_args.size());
    }


    //! @brief  空か
    bool Arguments::empty()const noexcept {
        return m_args.empty();
    }

}


