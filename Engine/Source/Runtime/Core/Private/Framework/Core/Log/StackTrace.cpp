//***********************************************************
//! @file
//***********************************************************
#include <Framework/Core/Log/StackTrace.h>
#include <Framework/Core/CoreTypes.h>
#include <Framework/Core/String/StringEncoder.h>

namespace ob::core {

	StackTrace StackTrace::Capture(s32 frameToSkip) {
		StackTrace result;
#ifdef OB_DEBUG
		auto stacktraces = std::stacktrace::current();
		for (s32 i=frameToSkip;i<stacktraces.size();++i)
		{
			result.entries.push_back(stacktraces[i]);
			if (result.entries.size() == result.entries.capacity()) break;
		}
#endif
		return result;
	}

	auto StackTrace::elements()const->FixedVector<StackTraceElement,MAX_ENTRY_COUNT> {

		FixedVector<StackTraceElement,MAX_ENTRY_COUNT> result;

#ifdef OB_DEBUG
		for (auto& entry: entries)
		{
			auto& element = result.emplace_back();
			element.filename = entry.source_file();
			element.line = entry.source_line();
			element.name = entry.description();
		}
#endif

		return result;
	}

}
