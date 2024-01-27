//***********************************************************
//! @file
//! @brief		
//! @author		Gajumaru
//***********************************************************
#include <Framework/Graphics/FrameGraph/FGFrameBuffer.h>
#include <Framework/Graphics/FrameGraph/FGResourcePool.h>

namespace ob::graphics {


	void FGFrameBuffer::create(const Desc& desc, void* allocator) {
		OB_ASSERT_EXPR(allocator);
		instance = static_cast<FGResourcePool*>(allocator)->createFrameBuffer(desc);
	}
	void FGFrameBuffer::destroy(const Desc& desc, void* allocator) {
		OB_ASSERT_EXPR(allocator);
		OB_ASSERT_EXPR(instance);
		static_cast<FGResourcePool*>(allocator)->destroyFrameBuffer(desc,instance);
	}
	std::string FGFrameBuffer::toStringt(const Desc& desc) {
		U8String name;
		StringEncoder::Encode(desc.name, name);
		return name.str();
	}


}