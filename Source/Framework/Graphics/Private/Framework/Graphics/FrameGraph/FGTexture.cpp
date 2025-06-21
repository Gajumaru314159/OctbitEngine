//***********************************************************
//! @file
//! @brief		
//! @author		Gajumaru
//***********************************************************
#include <Framework/Graphics/FrameGraph/FGTexture.h>
#include <Framework/Graphics/FrameGraph/FGResourcePool.h>
#include <magic_enum.hpp>

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
	std::string FGTexture::toString(const Desc& desc) {
		bool isColorFormat = !(TextureFormatUtility::HasDepth(desc.format) || TextureFormatUtility::HasStencil(desc.format));
		auto str = Format(
			"Name  :{}\n"
			"Format:{}\n"
			"Size  :{}x{}\n"
			"Clear :",
			desc.name,
			magic_enum::enum_name(desc.format),
			desc.size.width,
			desc.size.height
		);
		if (isColorFormat) {
			str += Format("{}",desc.clear.color);
		} else {
			str += Format("{}/{}", desc.clear.depth,desc.clear.stencil);
		}
		return std::move(str);
	}

	void FGTexture::preRead(const Desc& desc, uint32_t flags, void* ctx) {
		auto& cmd = *static_cast<Ref<rhi::CommandList>*>(ctx);
	}
	void FGTexture::preWrite(const Desc& desc, uint32_t flags, void* ctx) {
		auto& cmd = *static_cast<Ref<rhi::CommandList>*>(ctx);
	}

}