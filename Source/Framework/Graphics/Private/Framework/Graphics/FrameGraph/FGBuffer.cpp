//***********************************************************
//! @file
//! @brief		
//! @author		Gajumaru
//***********************************************************
#include <Framework/Graphics/FrameGraph/FGBuffer.h>
#include <Framework/Graphics/FrameGraph/FGResourcePool.h>
#include <magic_enum.hpp>

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
	std::string FGBufferInstance::toString(const Desc& desc) {
		auto str = Format(
			"Name  :{}\n"
			"Type  :{}\n"
			"Usaege:{}\n"
			"Size  :{}\n"
			"Stride:{}",
			desc.name,
			magic_enum::enum_name(desc.type),
			magic_enum::enum_name(desc.type),
			desc.size,
			desc.stride
		);
		return std::move(str);
	}

}