//***********************************************************
//! @file
//***********************************************************
#include <Framework/Core/Log/StackTrace.h>
#include <Framework/Core/CoreTypes.h>
#include <Framework/Core/String/StringEncoder.h>

#ifdef OS_LINUX

#ifdef OB_DEBUG
#include <execinfo.h>
#endif

namespace ob::core {

	StackTrace StackTrace::Capture(s32 frameToSkip) {
		StackTrace result;
#ifdef OB_DEBUG
		constexpr int MAX_FRAMES = 64;
		void* buffer[MAX_FRAMES];

		int traceCount = backtrace(buffer, std::min(MAX_FRAMES,MAX_DEPTH+frameToSkip));

		result.depth = std::max(traceCount - frameToSkip,0);;

		for (s32 i =frameToSkip; i < traceCount; i++) {
			result.stack[i-frameToSkip] = buffer[i];
		}
#endif
		return result;
	}

	auto StackTrace::elements()const->Vector<StackTraceElement> {

		Vector<StackTraceElement> result;

#ifdef OB_DEBUG
		if (char** symbols = backtrace_symbols(stack, depth)) 		{
			for (s32 i=0;i<depth;++i) {
				auto& element = result.emplace_back();
				element.filename = symbols[i];
			}
		}
#endif

		return result;
	}

}
#endif
