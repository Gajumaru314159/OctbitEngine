//***********************************************************
//! @file
//! @brief		
//! @author		Gajumaru
//***********************************************************
#include <Framework/Graphics/FrameGraph/FGTexture.h>
#include <Framework/Graphics/FrameGraph/FGResourcePool.h>

namespace ob::graphics {


	void FGTexture::create(const Desc& desc, void* allocator) {
		OB_ASSERT_EXPR(allocator);
		instance = static_cast<FGResourcePool*>(allocator)->createTexture(desc);
	}
	void FGTexture::destroy(const Desc& desc, void* allocator) {
		OB_ASSERT_EXPR(allocator);
		OB_ASSERT_EXPR(instance);
		static_cast<FGResourcePool*>(allocator)->destroyTexture(desc,instance);
	}
	std::string FGTexture::toStringt(const Desc& desc) {
		U8String name;
		StringEncoder::Encode(desc.name, name);
		return name.str();
	}


}