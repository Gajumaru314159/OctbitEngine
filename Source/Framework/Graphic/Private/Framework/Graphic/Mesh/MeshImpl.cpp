//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include <Framework/Graphic/Mesh/MeshImpl.h>

namespace ob::graphic {

	//@―---------------------------------------------------------------------------
	//!	@brief			生成
	//@―---------------------------------------------------------------------------
	Ref<Mesh> Mesh::Create(StringView name) {
		return new MeshImpl(name);
	}


	//@―---------------------------------------------------------------------------
	//!	@brief			コンストラクタ
	//@―---------------------------------------------------------------------------
	MeshImpl::MeshImpl(StringView name) {
		m_name = name;
	}

	//===============================================================
	// 設定
	//===============================================================

	//@―---------------------------------------------------------------------------
	//!	@brief			頂点レイアウトを設定
	//! @details		setVertices で設定されるBlobの解釈方法を設定します。
	//@―---------------------------------------------------------------------------
	void MeshImpl::setInputLayout(const rhi::VertexLayout& layout) {
		m_layout = layout;

		m_stribe = 0;

	}

	//@―---------------------------------------------------------------------------
	//!	@brief			頂点を設定
	//@―---------------------------------------------------------------------------
	void MeshImpl::setVertices(BlobView blob, s32 stribe, s32 start) {
		auto offset = (size_t)stribe * start;
		auto size = blob.size() + offset;
		if (!m_vertexBuffer || m_vertexBuffer->getDesc().bufferSize < offset) {
			auto desc = rhi::BufferDesc::Vertex<f32>(0);

			desc.name = m_name;
			desc.bufferSize = size;
			desc.bufferStride = stribe;

			m_vertexBuffer = rhi::Buffer::Create(desc);
		}
		m_vertexBuffer->update(blob.size(), blob.data(), offset);
	}

	//@―---------------------------------------------------------------------------
	//! @brief			インデックスバッファを設定
	//! @param blob		頂点データのバイナリオブジェクト
	//! @param start	頂点データの書き込み先
	//! @param is32bit	32bitインデックスを使用するか
	//@―---------------------------------------------------------------------------
	void MeshImpl::setIndices(BlobView blob, s32 start, bool is32bit) {
		auto stribe = is32bit ? sizeof(u32) : sizeof(u16);
		auto offset = start * stribe;
		auto size = blob.size() + offset;
		if (!m_indexBuffer || m_indexBuffer->getDesc().bufferSize < offset) {
			auto desc = rhi::BufferDesc::Index<u16>(0);

			desc.name = m_name;
			desc.bufferSize = size;
			desc.bufferStride = stribe;

			m_indexBuffer = rhi::Buffer::Create(desc);
		}
		m_indexBuffer->update(blob.size(), blob.data(), offset);

	}

	//@―---------------------------------------------------------------------------
	//!	@brief			
	//@―---------------------------------------------------------------------------
	void MeshImpl::setSubMeshes(Span<SubMesh> submeshes) {
		m_submeshes = Array<SubMesh>(submeshes.begin(), submeshes.end());
	}

	//@―---------------------------------------------------------------------------
	//!	@brief			CPU上のメッシュデータを整形してGPUに転送
	//@―---------------------------------------------------------------------------
	void MeshImpl::apply() {
		OB_NOTIMPLEMENTED();
	}

	//@―---------------------------------------------------------------------------
	//!	@brief			CPU上のメッシュのコピーを解放
	//! @details		この関数を呼び出すと positions() や colors() からメッシュの情報に
	//!					アクセスできなくなります。
	//@―---------------------------------------------------------------------------
	void MeshImpl::clear() {
		OB_NOTIMPLEMENTED();
	}


	//===============================================================
	// 再計算
	//===============================================================

	//@―---------------------------------------------------------------------------
	//!	@brief			頂点からメッシュのバウンディングボリュームを再計算します。
	//@―---------------------------------------------------------------------------
	void MeshImpl::recalculateBounds() {
		OB_NOTIMPLEMENTED();
	}

	//@―---------------------------------------------------------------------------
	//!	@brief			三角形と頂点からメッシュの法線を再計算
	//! @brief			頂点を変更した後、変更を反映させるために法線を更新することがしばしば必要です。
	//!					法線はすべての共有される頂点によって計算されます。
	//@―---------------------------------------------------------------------------
	void MeshImpl::recalculateNormals() {
		OB_NOTIMPLEMENTED();
	}

	//@―---------------------------------------------------------------------------
	//!	@brief			法線とテクスチャ座標からメッシュの接線を再計算
	//! @brief			メッシュの頂点と法線を変更した後、法線マップを参照する
	//!					シェーダーを使用してメッシュをレンダリングする場合は、
	//!					接線を更新する必要があります。
	//!					接線は、メッシュの頂点位置、法線、およびテクスチャ座標を使用して計算されます。
	//@―---------------------------------------------------------------------------
	void MeshImpl::recalculateTangents() {
		OB_NOTIMPLEMENTED();
	}


}// namespace ob