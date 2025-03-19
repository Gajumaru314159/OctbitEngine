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

}