//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#include <Framework/RHI/GraphicFile.h>
#include <Framework/RHI/RHI.h>

namespace ob::rhi {

	Ref<GraphicFileHandle> GraphicFileHandle::Create(StringView path) {
		if (auto device = Device::Get()) {
			return device->createGraphicFileHandle(path);
		}
		return nullptr;
	}

	Ref<GraphicFileEvent> GraphicFileEvent::Create() {
		if (auto device = Device::Get()) {
			return device->createGraphicFileEvent();
		}
		return nullptr;
	}

	Ref<GraphicFileQueue> GraphicFileQueue::Create(const GraphicFileQueueDesc& desc) {
		if (auto device = Device::Get()) {
			return device->createGraphicFileQueue(desc);
		}
		return nullptr;
	}


	bool GraphicFile::Generate(StringView input, StringView output,s32 compressionLevel) {
		if (auto device = Device::Get()) {
			return device->generateGraphicFile(input,output,compressionLevel);
		}
		return false;
	}

	Vector<GraphicFileMipInfo> GraphicFile::Prepare(StringView file) {
		if (auto device = Device::Get()) {
			return device->prepareGraphicFile(file);
		}
		return {};
	}
}