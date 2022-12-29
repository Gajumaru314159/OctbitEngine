//***********************************************************
//! @file
//! @brief		リソース・バリア
//! @author		Gajumaru
//***********************************************************
#include <Framework/RHI/Types/ResourceState.h>
#include <Framework/RHI/Texture.h>

#pragma once
namespace ob::rhi {
	class Texture;
}

namespace ob::rhi {

	//@―---------------------------------------------------------------------------
	//! @brief  リソース・バリア
	//@―---------------------------------------------------------------------------
	class ResourceBarrier {
	public:

		//===============================================================
		// コンストラクタ / デストラクタ
		//===============================================================

		//@―---------------------------------------------------------------------------
		//! @brief  説明
		//@―---------------------------------------------------------------------------
		void add(const Texture& texture, ResourceState prev, ResourceState next,s32 arrayIndex =-1,s32 mipIndex=-1);

	public:

		struct TextureBarrier {
			const Texture* pTexture;
			ResourceState prev;
			ResourceState next;
			s32 mipIndex;
			s32 arrayIndex;
		};

		Array<TextureBarrier> m_textureBarriers;

	};

}// namespcae ob::rhi