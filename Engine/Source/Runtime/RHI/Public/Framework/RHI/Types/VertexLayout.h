//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/Constants.h>

namespace ob::rhi {

	//! @brief		型
	//! 
	//! @see		VertexAttribute
	//! @see		VertexLayout
	enum class ElementType : u32 {
		Int8,
		Int16,
		Int32,
		UInt8,
		UInt16,
		UInt32,
		Float,
		Int8Norm,
		UInt8Norm,
	};


	//! @brief		セマンティクス
	//! 
	//! @details	Positionは頂点内で1度しか使用できません。
	//! @see		VertexAttribute
	//! @see		VertexLayout
	enum class Semantic : u32 {
		Position,		//!< 頂点
		Normal,			//!< 法線
		Binormal,		//!< 従法線
		Tangent,		//!< 接線
		Color,			//!< 色
		TexCoord,		//!< テクスチャ座標(UV座標)
		BlendIndices,	//!< ブレンド・インデックス
		BlendWeights,	//!< ブレンドの重み
		PointSize,		//!< ポイントサイズ
	};

	enum class VertexInputRate : u32 {
		Vertex,			//!< 頂点ごと
		Instance,		//!< インスタンスごと
	};

	//! @brief  頂点属性
	//! @see    VertexLayout
	struct VertexAttribute {

		Semantic		semantic;	//!< セマンティクス
		s32				offset;		//!< 頂点構造体内でのオフセット
		ElementType		type;		//!< コンポーネント型
		s32				dimension;	//!< 次元数
		s32				index;		//!< セマンティクス内インデックス
		VertexInputRate inputRate = VertexInputRate::Vertex;	//!< 頂点ごとかインスタンスごとか

	public:

		//! @brief      コンストラクタ
		VertexAttribute() = default;

		//! @brief      コンストラクタ
		VertexAttribute(Semantic semantic, s32 offset, ElementType type, s32 dimension = 1, s32 index = 0)
			:semantic(semantic), offset(offset), type(type), dimension(dimension), index(index) {}

	};

	//! @brief  頂点属性・配列
	using VertexAttributeArray = FixedVector<VertexAttribute, VERTEX_ATTRIBUTE_MAX>;


	//! @brief  頂点レイアウト
	struct VertexLayout {
		VertexAttributeArray attributes;			//!< 属性リスト
		s32					 vertexStride = 0;		//!< ストライド幅
		s32					 instanceStride = 0;	//!< ストライド幅
	};

}