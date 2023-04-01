//***********************************************************
//! @file
//! @brief		スタックトレース
//***********************************************************
#pragma once
#include <Framework/Core/Log/StackTrace.h>
#include <Framework/Core/CoreTypes.h>
#include <Framework/Core/String/StringEncoder.h>

#ifdef OB_DEBUG

#ifdef OS_WINDOWS
#include <Windows.h>
#include <DbgHelp.h>
#pragma comment(lib, "DbgHelp.lib")

#endif

#endif

namespace ob::core {

    static const int MAX_FRAMES_TO_CAPTURE = 256;
    static const int MAX_SYMBOL_NAME_LEN = MAX_PATH;

	StackTrace::StackTrace() {

#ifdef OB_DEBUG

#ifdef OS_WINDOWS

		auto process = ::GetCurrentProcess();
        auto success = ::SymInitialize(process, NULL, TRUE);


        u8* symbol[sizeof(SYMBOL_INFO) + MAX_SYMBOL_NAME_LEN];
        {
            auto& symbolInfo = *reinterpret_cast<SYMBOL_INFO*>(symbol);
            symbolInfo.SizeOfStruct = sizeof(SYMBOL_INFO);
            symbolInfo.MaxNameLen = MAX_SYMBOL_NAME_LEN;
        }

        std::string result;

        void* stack[MAX_FRAMES_TO_CAPTURE];
        const WORD frames = ::RtlCaptureStackBackTrace(1, MAX_FRAMES_TO_CAPTURE, stack, NULL);

        for (WORD i = 0; i < frames; i++)
        {

            if (!::SymFromAddr(process, (DWORD64)(stack[i]), 0, reinterpret_cast<SYMBOL_INFO*>(symbol))) {
                continue;
            }

            DWORD disp = 0;
            IMAGEHLP_LINE64 line{};
            if (!::SymGetLineFromAddr64(process, (DWORD64)(stack[i]), &disp, &line)) {
                continue;
            }

            auto& symbolInfo = *reinterpret_cast<SYMBOL_INFO*>(symbol);

            auto& element = m_stack.emplace_back();
            StringEncoder::Encode(StringViewBase<CHAR>(&symbolInfo.Name[0], symbolInfo.NameLen), element.name);
            StringEncoder::Encode(line.FileName, element.filename);
            element.line = line.LineNumber;
        }

        if (success) {
            ::SymCleanup(process);
        }

#else
#pragma error("Unsupported")
#endif

#endif
	}


}