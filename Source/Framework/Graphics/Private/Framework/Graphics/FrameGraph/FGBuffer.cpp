//***********************************************************
//! @file
//! @brief		
//! @author		Gajumaru
//***********************************************************
#include <Framework/Graphics/FrameGraph/FGBuffer.h>
#include <Framework/Graphics/FrameGraph/FGResourcePool.h>

namespace ob::graphics {

	void FGBufferInstance::create(const Desc& desc, void* allocator) {
		OB_ASSERT_EXPR(allocator);
		instance = static_cast<FGResourcePool*>(allocator)->createBuffer(desc);
	}
	void FGBufferInstance::destroy(const Desc& desc, void* allocator) {
		OB_ASSERT_EXPR(allocator);
		OB_ASSERT_EXPR(instance);
		static_cast<FGResourcePool*>(allocator)->destroyBuffer(desc, instance);
	}
	std::string FGBufferInstance::toStringt(const Desc& desc) {
		U8String name;
		StringEncoder::Encode(desc.name, name);
		return name.str();
	}

}