//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#include <Framework/RHI/GraphicFile.h>
#include <Framework/RHI/RHI.h>

namespace ob::rhi {

	Ref<GraphicFileHandle> GraphicFileHandle::Create(StringView path) {
		if (auto rhi = Device::Get()) {
			return rhi->createGraphicFileHandle(path);
		}
		return nullptr;
	}

	Ref<GraphicFileEvent> GraphicFileEvent::Create() {
		if (auto rhi = Device::Get()) {
			return rhi->createGraphicFileEvent();
		}
		return nullptr;
	}

	Ref<GraphicFileQueue> GraphicFileQueue::Create(const GraphicFileQueueDesc& desc) {
		if (auto rhi = Device::Get()) {
			return rhi->createGraphicFileQueue(desc);
		}
		return nullptr;
	}


	bool GraphicFile::Generate(StringView input, StringView output,s32 compressionLevel) {
		if (auto rhi = Device::Get()) {
			return rhi->generateGraphicFile(input,output,compressionLevel);
		}
		return false;
	}

	Vector<GraphicFileMipInfo> GraphicFile::Prepare(StringView file) {
		if (auto rhi = Device::Get()) {
			return rhi->prepareGraphicFile(file);
		}
		return {};
	}
}