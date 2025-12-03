//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/DescriptorTable.h>

namespace ob::rhi {

	//! @brief  デスクリプタ・テーブル・コマンドパラメータ
	struct SetDescriptorTableParam {
		Ref<DescriptorTable> table;
		s32				slot;
	public:
		SetDescriptorTableParam() = default;
		SetDescriptorTableParam(const Ref<DescriptorTable>& table,s32 slot)
			:table(table),slot(slot)
		{}
	};


	//! @brief  CommandList::draw() で指定するパラメータ
	struct DrawParam {
		u32 startVertex = 0;	//!< 頂点開始インデックス
		u32 vertexCount = 0;	//!< 描画頂点数
		u32 startInstance = 0;	//!< インスタンス開始インデックス
		u32 instanceCount = 1;	//!< インスタンス数
	};


	//! @brief  CommandList::drawIndexed() で指定するパラメータ
	struct DrawIndexedParam {
		u32 startVertex = 0;	//!< 頂点開始インデックス
		u32 startIndex = 0;		//!< インデックス開始インデックス
		u32 indexCount = 0;		//!< 描画頂点数
		u32 startInstance = 0;	//!< インスタンス開始インデックス
		u32 instanceCount = 1;	//!< インスタンス数
	};


	//! @brief  
	struct TextureSubresource {
		u32 arrayIndex;
		u32 faceIndex;
		u32 mipLevel;
		std::optional<IntRect> region;
	};


	//! @brief  
	struct CopyTextureParam {
		TextureSubresource src;		//!< コピー元
		TextureSubresource dest;	//!< コピー先
	};


	//! @brief  
	struct SetRootConstantsParam {
		s32			offset;	//!< 書き込むオフセット
		BlobView	blob;	//!< 書き込むバイナリ
	};

}