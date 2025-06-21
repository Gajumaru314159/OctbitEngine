//***********************************************************
//! @file
//! @brief		
//! @author		Gajumaru
//***********************************************************
#include <Framework/Graphics/FrameGraph/FGBuffer.h>
#include <Framework/Graphics/FrameGraph/FGResourcePool.h>
#include <magic_enum.hpp>

namespace ob::graphics {

	void FGBuffer::create(const Desc& desc, void* allocator) {
		OB_ASSERT_EXPR(allocator);
		instance = static_cast<FGResourcePool*>(allocator)->createBuffer(desc);
	}
	void FGBuffer::destroy(const Desc& desc, void* allocator) {
		OB_ASSERT_EXPR(allocator);
		OB_ASSERT_EXPR(instance);
		static_cast<FGResourcePool*>(allocator)->destroyBuffer(desc, instance);
	}
	std::string FGBuffer::toString(const Desc& desc) {
		auto str = Format(
			"Name  :{}\n"
			"Type  :{}\n"
			"Usaege:{}\n"
			"Size  :{}\n"
			"Stride:{}",
			desc.name,
			magic_enum::enum_name(desc.state),
			magic_enum::enum_name(desc.state),
			desc.size,
			desc.stride
		);
		return std::move(str);
	}

	void FGBuffer::preRead(const Desc& desc, uint32_t flags, void* ctx) {
		auto& cmd = *static_cast<Ref<rhi::CommandList>*>(ctx);
	}
	void FGBuffer::preWrite(const Desc& desc, uint32_t flags, void* ctx) {
		auto& cmd = *static_cast<Ref<rhi::CommandList>*>(ctx);
	}

}