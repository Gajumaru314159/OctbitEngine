//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include <Framework/Graphics/MeshData.h>
namespace ob::graphics {

	//@―---------------------------------------------------------------------------
	//!	@brief			頂点からメッシュのバウンディングボリュームを再計算します。
	//@―---------------------------------------------------------------------------
	void MeshData::recalculateBoundsInCPU() {
		for (auto& submesh : submeshes) {

			submesh.bounds = Box::Empty;

			if (submesh.indexStart < 0) {
				LOG_ERROR("メッシュ領域の計算に失敗。インデックス開始位置が不正です。[indexStart={}]",submesh.indexStart);
				continue;
			}

			if (indices.size() <= (size_t)submesh.indexStart + submesh.indexCount) {
				LOG_ERROR("メッシュ領域の計算に失敗。インデックスが頂点数を超えました。[indexStart={},indexCoun={}]",submesh.indexStart,submesh.indexCount);
				continue;
			}

			s32 j = submesh.indexStart;
			for (s32 i = 0; i < submesh.indexCount; ++i) {

				auto index = indices[j] + submesh.baseVertex;

				if (!is_in_range(index,positions)) {
					LOG_ERROR("メッシュ領域の計算に失敗。インデックスが範囲外です。[index={},positions.size={}]",index,positions.size());
					continue;
				}

				submesh.bounds += positions[index];

				j++;
			}

		}

	}

	//@―---------------------------------------------------------------------------
	//!	@brief			三角形と頂点からメッシュの法線を再計算
	//! @brief			頂点を変更した後、変更を反映させるために法線を更新することがしばしば必要です。
	//!					法線はすべての共有される頂点によって計算されます。
	//@―---------------------------------------------------------------------------
	void MeshData::recalculateNormalsInCPU() {
		OB_NOTIMPLEMENTED();
	}

	//@―---------------------------------------------------------------------------
	//!	@brief			法線とテクスチャ座標からメッシュの接線を再計算
	//! @brief			メッシュの頂点と法線を変更した後、法線マップを参照する
	//!					シェーダーを使用してメッシュをレンダリングする場合は、
	//!					接線を更新する必要があります。
	//!					接線は、メッシュの頂点位置、法線、およびテクスチャ座標を使用して計算されます。
	//@―---------------------------------------------------------------------------
	void MeshData::recalculateTangentsInCPU() {
		OB_NOTIMPLEMENTED();
	}

}