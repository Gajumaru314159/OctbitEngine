﻿//***********************************************************
//! @file
//! @brief		メッシュバッファ
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/Buffer.h>
#include <Framework/RHI/MeshData.h>
#include <Framework/RHI/Types/PipelineStateDesc.h>

namespace ob::graphic {

	struct BoneWeight {
		s32 boneIndices[4];
		f32 weights[4];
	};

	//@―---------------------------------------------------------------------------
	//!	@brief		サブメッシュ
	//! @details	メッシュ内の描画単位を表します。
	//!				```v = vertices[ indices[indexStart + i] + baseVertex]```
	//@―---------------------------------------------------------------------------
	struct SubMesh {
		Box		bounds;
		s32		indexStart = 0;		//!< このサブメッシュのインデックスバッファの開始地点
		s32		indexCount = 0;		//!< このサブメッシュのインデックス数
		s32		baseVertex = 0;		//!< インデックスバッファの各値に追加されるオフセット
		//s32 firstVertex;		//!< index-0 が参照する頂点のインデックス ```min(indices)```
		//s32 vertexCount;		//!< このサブメッシュのインデックスバッファによって使用される頂点の数 ```max(indices)-min(indices)+1```
		rhi::Topology topology = rhi::Topology::TriangleList;
	};

	struct MeshData {
	public:
		String				name;

		//===============================================================
		// インデックス
		//===============================================================
		Array<u16>			indices;
		Array<u32>			indices32;

		//===============================================================
		// 頂点
		//===============================================================
		Array<Vec3>			positions;
		Array<Color>		colors;
		Array<IntColor>		intColors;
		Array<Vec3>			normals;
		Array<Vec4>			tangents;
		Array<Vec2>			uvs;
		Array<Vec2>			uvs1;
		Array<Vec2>			uvs2;
		Array<Vec2>			uvs3;
		Array<Vec2>			uvs4;
		Array<Vec2>			uvs5;
		Array<Vec2>			uvs6;
		Array<Vec2>			uvs7;
		//Array<Matrix>		bindPoses;
		//Array<BoneWeight>	boneWeights;

		//===============================================================
		// サブメッシュ
		//===============================================================
		Array<SubMesh>		submeshes;
	public:

		//@―---------------------------------------------------------------------------
		//!	@brief			頂点からメッシュのバウンディングボリュームを再計算します。
		//@―---------------------------------------------------------------------------
		void recalculateBoundsInCPU();

		//@―---------------------------------------------------------------------------
		//!	@brief			三角形と頂点からメッシュの法線を再計算
		//! @brief			頂点を変更した後、変更を反映させるために法線を更新することがしばしば必要です。
		//!					法線はすべての共有される頂点によって計算されます。
		//@―---------------------------------------------------------------------------
		void recalculateNormalsInCPU();

		//@―---------------------------------------------------------------------------
		//!	@brief			法線とテクスチャ座標からメッシュの接線を再計算
		//! @brief			メッシュの頂点と法線を変更した後、法線マップを参照する
		//!					シェーダーを使用してメッシュをレンダリングする場合は、
		//!					接線を更新する必要があります。
		//!					接線は、メッシュの頂点位置、法線、およびテクスチャ座標を使用して計算されます。
		//@―---------------------------------------------------------------------------
		void recalculateTangentsInCPU();

	};

}// namespcae ob