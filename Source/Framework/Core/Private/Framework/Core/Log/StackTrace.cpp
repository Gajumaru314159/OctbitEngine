//***********************************************************
//! @file
//! @brief		スタックトレース
//***********************************************************
#include <Framework/Core/Log/StackTrace.h>
#include <Framework/Core/CoreTypes.h>
#include <Framework/Core/String/StringEncoder.h>

#ifdef OB_DEBUG

#ifdef OS_WINDOWS
#define NOMINMAX
#include <Windows.h>
#include <DbgHelp.h>
#pragma comment(lib, "DbgHelp.lib")

#endif

#endif

namespace ob::core {

#ifdef OS_WINDOWS

	StackTrace StackTrace::Capture(s32 frameToSkip) {
		StackTrace result;
#ifdef OB_DEBUG
		result.depth = ::RtlCaptureStackBackTrace(frameToSkip + 1, std::size(result.stack), result.stack, NULL);
#endif
		return result;
	}

	auto StackTrace::elements()const->Vector<StackTraceElement> {

		Vector<StackTraceElement> result;

#ifdef OB_DEBUG

		u8* symbol[sizeof(SYMBOL_INFO) + MAX_PATH];
		{
			auto& symbolInfo = *reinterpret_cast<SYMBOL_INFO*>(symbol);
			symbolInfo.SizeOfStruct = sizeof(SYMBOL_INFO);
			symbolInfo.MaxNameLen = MAX_PATH;
		}

		HANDLE process = ::GetCurrentProcess();

		auto limit = std::min<s32>(depth, std::size(stack));

		DWORD disp = 0;
		IMAGEHLP_LINE64 line{};

		for (s32 i = 0; i < limit; ++i) {
			if (!::SymFromAddr(process, (DWORD64)(stack[i]), 0, reinterpret_cast<SYMBOL_INFO*>(symbol))) {
				continue;
			}
			if (!::SymGetLineFromAddr64(process, (DWORD64)(stack[i]), &disp, &line)) {
				continue;
			}

			auto& symbolInfo = *reinterpret_cast<SYMBOL_INFO*>(symbol);

			auto& element = result.emplace_back();
			StringEncoder::Encode(StringViewBase<CHAR>(&symbolInfo.Name[0], symbolInfo.NameLen), element.name);
			StringEncoder::Encode(line.FileName, element.filename);
			element.line = line.LineNumber;
		}
#endif
		return result;
	}


#else

#pragma error("Unsupported")

#endif

}