//***********************************************************
//! @file
//! @brief		
//! @author		Gajumaru
//***********************************************************
#include <Framework/RHI/GraphicFile.h>
#include <Framework/RHI/RHI.h>

namespace ob::rhi {

	Ref<GraphicFileHandle> GraphicFileHandle::Create(StringView path) {
		if (auto rhi = RHI::Get()) {
			return rhi->createGraphicFileHandle(path);
		}
		return nullptr;
	}

	Ref<GraphicFileEvent> GraphicFileEvent::Create() {
		if (auto rhi = RHI::Get()) {
			return rhi->createGraphicFileEvent();
		}
		return nullptr;
	}

	Ref<GraphicFileQueue> GraphicFileQueue::Create(const GraphicFileQueueDesc& desc) {
		if (auto rhi = RHI::Get()) {
			return rhi->createGraphicFileQueue(desc);
		}
		return nullptr;
	}


	bool GraphicFile::Generate(StringView input, StringView output) {
		if (auto rhi = RHI::Get()) {
			return rhi->generateGraphicFile(input,output);
		}
		return false;
	}

	Vector<GraphicFileMipInfo> GraphicFile::Prepare(StringView file) {
		if (auto rhi = RHI::Get()) {
			return rhi->prepareGraphicFile(file);
		}
		return {};
	}
}