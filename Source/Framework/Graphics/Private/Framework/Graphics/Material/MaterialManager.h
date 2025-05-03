//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/Forward.h>
#include <Framework/RHI/Types/PipelineStateDesc.h>
#include <Framework/Graphics/Material/MaterialInternalTypes.h>
#include <Framework/Graphics/Material/MaterialBlock.h>
#include <Framework/RHI/DescriptorLayout.h>

namespace ob::graphics {

	//! @brief  説明
	class MaterialManager : public Singleton<MaterialManager> {
	public:

		MaterialManager(rhi::RHI&, NameDictionary&);
		~MaterialManager();

		//!	@brief			レイアウトID取得
		VertexLayoutId getVertexLayoutId(const rhi::VertexLayout& layout);

	public:

		void recordGlobalShaderProperties(Ref<rhi::CommandList>&);

	private:

		struct VertexLayoutPred {
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

	private:

		SpinLock m_lock;
		Map<rhi::VertexLayout, VertexLayoutId, VertexLayoutPred> m_vertexLayoutCache;

	};

}