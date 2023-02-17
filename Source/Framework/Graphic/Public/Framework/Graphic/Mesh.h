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

	struct Bounds {
		Vec3 min;
		Vec3 max;
	};

	struct Vertex {
		Matrix bindPose;		// 
		BoneWeight boneWeighr;	//
		Color color; // or IntColor
		Vec3 position;
		Vec3 normal;
		Vec4 tangent;	// wはbinormal用
		Vec2 uv;
		Vec2 uv2;
		Vec2 uv3;
		Vec2 uv4;
		Vec2 uv5;
		Vec2 uv6;
		Vec2 uv7;
		Vec2 uv8;
	};

	//@―---------------------------------------------------------------------------
	//!	@brief
	//! @details	```v = vertices[ indices[indexStart + i] + baseVertex]```
	//@―---------------------------------------------------------------------------
	struct SubMesh {
		s32 indexStart = 0;		//!< このサブメッシュのインデックスバッファの開始地点
		s32 indexCount = 0;		//!< このサブメッシュのインデックス数
		s32 baseVertex = 0;		//!< インデックスバッファの各値に追加されるオフセット
		//s32 firstVertex;		//!< index-0 が参照する頂点のインデックス ```min(indices)```
		//s32 vertexCount;		//!< このサブメッシュのインデックスバッファによって使用される頂点の数 ```max(indices)-min(indices)+1```
		rhi::Topology topology = rhi::Topology::TriangleList;
	};

	//@―---------------------------------------------------------------------------
	//! @brief		メッシュ
	//! @details	メッシュには複数のサブメッシュが含まれます。
	//!				サブメッシュの数は描画に使用するマテリアル数と一致します。
	//!				複数のサブメッシュを使用せずにマテリアル毎にMeshを分ける場合は
	//!				GPU上のバッファも分割されます。
	//@―---------------------------------------------------------------------------
	class Mesh : public RefObject {
	public:

		static Ref<Mesh> Create(StringView name);

	public:
		//===============================================================
		// 設定
		//===============================================================

		//@―---------------------------------------------------------------------------
		//!	@brief			頂点レイアウトを設定
		//! @details		setVertices で設定されるBlobの解釈方法を設定します。
		//@―---------------------------------------------------------------------------
		virtual void setInputLayout(const rhi::VertexLayout& layout) = 0;

		//@―---------------------------------------------------------------------------
		//!	@brief			頂点を設定
		//@―---------------------------------------------------------------------------
		virtual void setVertices(BlobView blob,s32 stribe,s32 start=0) = 0;

		//@―---------------------------------------------------------------------------
		//! @brief			インデックスバッファを設定
		//! @param blob		頂点データのバイナリオブジェクト
		//! @param start	頂点データの書き込み先
		//! @param is32bit	32bitインデックスを使用するか
		//@―---------------------------------------------------------------------------
		virtual void setIndices(BlobView blob, s32 start, bool is32bit) = 0;

		//@―---------------------------------------------------------------------------
		//!	@brief			
		//@―---------------------------------------------------------------------------
		virtual void setSubMeshes(Span<SubMesh> submeshes) = 0;

		//@―---------------------------------------------------------------------------
		//!	@brief			CPU上のメッシュデータを整形してGPUに転送
		//@―---------------------------------------------------------------------------
		virtual void apply() = 0;

		//@―---------------------------------------------------------------------------
		//!	@brief			CPU上のメッシュのコピーを解放
		//! @details		この関数を呼び出すと positions() や colors() からメッシュの情報に
		//!					アクセスできなくなります。
		//@―---------------------------------------------------------------------------
		virtual void clear() = 0;

		
		//===============================================================
		// 再計算
		//===============================================================
		
		//@―---------------------------------------------------------------------------
		//!	@brief			頂点からメッシュのバウンディングボリュームを再計算します。
		//@―---------------------------------------------------------------------------
		virtual void recalculateBounds() = 0;

		//@―---------------------------------------------------------------------------
		//!	@brief			三角形と頂点からメッシュの法線を再計算
		//! @brief			頂点を変更した後、変更を反映させるために法線を更新することがしばしば必要です。
		//!					法線はすべての共有される頂点によって計算されます。
		//@―---------------------------------------------------------------------------
		virtual void recalculateNormals() = 0;

		//@―---------------------------------------------------------------------------
		//!	@brief			法線とテクスチャ座標からメッシュの接線を再計算
		//! @brief			メッシュの頂点と法線を変更した後、法線マップを参照する
		//!					シェーダーを使用してメッシュをレンダリングする場合は、
		//!					接線を更新する必要があります。
		//!					接線は、メッシュの頂点位置、法線、およびテクスチャ座標を使用して計算されます。
		//@―---------------------------------------------------------------------------
		virtual void recalculateTangents() = 0;


		//===============================================================
		// 取得
		//===============================================================

		//virtual s32 blendShapeCount() = 0;
		//virtual Bounds bounds() = 0;
		// indefFormat
		// isReadable
		// subMeshCount
		// attributeCount
		// bufferCount


		//virtual Array<Vec4> posiions() = 0;
		//virtual Array<s32> indices();



		//virtual s32 getIndexCount(s32 submesh)const = 0;
		//virtual s32 getIndexStart(s32 submesh)const = 0;
		//virtual Array<s32> getIndices(s32 submesh)const = 0;
		//virtual SubMesh getSubMesh(s32 index)const = 0;

	};

	inline void MeshTest() {

		auto mesh = Mesh::Create(TC("Mesh"));


		struct Vertex {
			Vec3 pos;
			Vec2 uv;
			Color color;
		};

		using namespace ob::rhi;

		rhi::VertexLayout layout{ {
				rhi::VertexAttribute(Semantic::Position, offsetof(Vertex,pos),Type::Float,3),
				rhi::VertexAttribute(Semantic::TexCoord, offsetof(Vertex,uv),Type::Float,2),
				rhi::VertexAttribute(Semantic::Color, offsetof(Vertex,color),Type::Float,4),
		} };
		Vertex vertices[]{
			{{0,0,0},{0,0},Color::White},
			{{0,0,0},{0,0},Color::White},
			{{0,0,0},{0,0},Color::White},
			{{0,0,0},{0,0},Color::White},
		};


		mesh->setInputLayout(layout);
		mesh->setVertices(vertices,sizeof(Vertex),0);
	}

}// namespcae ob