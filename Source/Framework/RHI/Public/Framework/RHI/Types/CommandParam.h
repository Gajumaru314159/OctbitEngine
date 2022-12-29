//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/DescriptorTable.h>

namespace ob::rhi {

	//@―---------------------------------------------------------------------------
	//! @brief  デスクリプタ・テーブル・コマンドパラメータ
	//@―---------------------------------------------------------------------------
	struct SetDescriptorTableParam {
		Ref<DescriptorTable> table;
		s32				slot;
	public:
		SetDescriptorTableParam() = default;
		SetDescriptorTableParam(const Ref<DescriptorTable> table,s32 slot)
			:table(table),slot(slot)
		{}
	};


	//@―---------------------------------------------------------------------------
	//! @brief  CommandList::draw() で指定するパラメータ
	//@―---------------------------------------------------------------------------
	struct DrawParam {
		u32 startVertex;	//!< 頂点開始インデックス
		u32 vertexCount;	//!< 描画頂点数
	};


	//@―---------------------------------------------------------------------------
	//! @brief  CommandList::drawIndexed() で指定するパラメータ
	//@―---------------------------------------------------------------------------
	struct DrawIndexedParam {
		u32 startVertex;	//!< 頂点開始インデックス
		u32 startIndex;		//!< インデックス開始インデックス
		u32 indexCount;		//!< 描画頂点数
	};


	//@―---------------------------------------------------------------------------
	//! @brief  CommandList::drawInstanced() で指定するパラメータ
	//@―---------------------------------------------------------------------------
	struct InstancedParam {
		u32 startInstance;	//!< インスタンス開始インデックス
		u32 instanceCount;	//!< インスタンス数
	};


	//@―---------------------------------------------------------------------------
	//! @brief  
	//@―---------------------------------------------------------------------------
	struct TextureSubresource {
		u32 arrayIndex;
		u32 faceIndex;
		u32 mipLevel;
		std::optional<IntRect> region;
	};


	//@―---------------------------------------------------------------------------
	//! @brief  
	//@―---------------------------------------------------------------------------
	struct CopyTextureParam {
		TextureSubresource src;		//!< コピー元
		TextureSubresource dest;	//!< コピー先
	};

}// namespcae ob::rhi