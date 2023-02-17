﻿//***********************************************************
//! @file
//! @brief		メッシュバッファ
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphic/Mesh.h>

namespace ob::graphic {

	//@―---------------------------------------------------------------------------
	//! @brief		メッシュ
	//! @details	メッシュには複数のサブメッシュが含まれます。
	//!				サブメッシュの数は描画に使用するマテリアル数と一致します。
	//!				複数のサブメッシュを使用せずにマテリアル毎にMeshを分ける場合は
	//!				GPU上のバッファも分割されます。
	//@―---------------------------------------------------------------------------
	class MeshImpl : public Mesh {
	public:

		//@―---------------------------------------------------------------------------
		//!	@brief			コンストラクタ
		//@―---------------------------------------------------------------------------
		MeshImpl(const MeshData&);
		MeshImpl(MeshData&&);

		//@―---------------------------------------------------------------------------
		//!	@brief			頂点レイアウトを設定
		//! @details		setVertices で設定されるBlobの解釈方法を設定します。
		//@―---------------------------------------------------------------------------
		const VertexLayout& getVertexLayout()const override;

		//@―---------------------------------------------------------------------------
		//!	@brief			メッシュデータを持っているか
		//@―---------------------------------------------------------------------------
		bool hasMeshData()const override;

		//@―---------------------------------------------------------------------------
		//!	@brief			メッシュデータを取得
		//! @details		```hasMeshData() == false```の場合空のMeshDataを返す。
		//@―---------------------------------------------------------------------------
		const MeshData& getMeshData()const override;

		//@―---------------------------------------------------------------------------
		//!	@brief			サブメッシュの数を取得
		//@―---------------------------------------------------------------------------
		s32 getSubMeshCount()const override;

		//@―---------------------------------------------------------------------------
		//!	@brief			指定したインデックスのサブメッシュを取得
		//@―---------------------------------------------------------------------------
		SubMesh getSubMesh(s32 index)const override;

		//@―---------------------------------------------------------------------------
		//!	@brief			サブメッシュのリストを取得
		//@―---------------------------------------------------------------------------
		const Array<SubMesh>& getSubMeshes()const override;


		void initLayoutFromMeshData(const MeshData&);

	private:

		rhi::VertexLayout	m_layout;

		MeshData			m_meshData;
		bool				m_initByMeshData = false;

		Ref<rhi::Buffer>	m_vertexBuffer;
		Ref<rhi::Buffer>	m_indexBuffer;
	};

}// namespcae ob