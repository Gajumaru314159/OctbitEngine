//***********************************************************
//! @file
//! @brief		メッシュバッファ
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphic/Mesh/MeshImpl.h>

namespace ob::graphic {

	//@―---------------------------------------------------------------------------
	//! @brief		頂点レイアウトマネージャ
	//@―---------------------------------------------------------------------------
	class VertexLayoutManager {
	public:

		VertexLayoutManager() = default;

		//@―---------------------------------------------------------------------------
		//!	@brief			レイアウトID取得
		//@―---------------------------------------------------------------------------
		VertexLayoutId getVertexLayoutId(const rhi::VertexLayout& layout) {
			ScopeLock lock(m_lock);
			auto found = m_map.find(layout);
			if (found == m_map.end()) {
				return m_map[layout] = static_cast<VertexLayoutId>(m_map.size());
			} else {
				return found->second;
			}
		}

	private:

		struct Pred {
			inline bool operator()(const rhi::VertexLayout& a, const rhi::VertexLayout& b)const {
				if (a.attributes.size() < b.attributes.size())return true;
				if (a.attributes.size() > b.attributes.size())return false;

				for (s32 i = 0; i < a.attributes.size(); ++i) {

					auto& aa = a.attributes[i];
					auto& ab = b.attributes[i];

					if (aa.semantic < ab.semantic) return true;
					if (aa.type < ab.type) return true;
					if (aa.dimention < ab.dimention) return true;
					if (aa.semantic < ab.semantic) return true;
					if (aa.index < ab.index) return true;

				}
				return false;
			}
		};

		SpinLock m_lock;
		Map<rhi::VertexLayout, VertexLayoutId,Pred> m_map;

	};

}// namespcae ob