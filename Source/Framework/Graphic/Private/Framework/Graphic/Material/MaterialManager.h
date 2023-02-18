//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/Forward.h>
#include <Framework/RHI/Types/SubPass.h>
#include <Framework/RHI/Types/PipelineStateDesc.h>
#include <Framework/RHI/RenderPass.h>
#include <Framework/Engine/Name.h>
#include <Framework/Graphic/Material/MaterialInternalTypes.h>

namespace ob::graphic {

	//@―---------------------------------------------------------------------------
	//! @brief  説明
	//@―---------------------------------------------------------------------------
	class MaterialManager{
	public:

		//@―---------------------------------------------------------------------------
		//!	@brief			取得
		//@―---------------------------------------------------------------------------
		static MaterialManager& Get();

	public:

		MaterialManager();
		~MaterialManager();

		//@―---------------------------------------------------------------------------
		//!	@brief			描画タグにRenderPassを登録
		//@―---------------------------------------------------------------------------
		void registerRenderPass(engine::Name name, const Ref<rhi::RenderPass>&, s32 subpass);

		//@―---------------------------------------------------------------------------
		//!	@brief			描画タグからRenderPassを登録
		//@―---------------------------------------------------------------------------
		rhi::SubPass FindRenderPass(engine::Name renderTag);


		//@―---------------------------------------------------------------------------
		//!	@brief			レイアウトID取得
		//@―---------------------------------------------------------------------------
		VertexLayoutId getVertexLayoutId(const rhi::VertexLayout& layout);

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

		HashMap<engine::Name, rhi::SubPass> m_renderPassMap;

		SpinLock m_lock;
		Map<rhi::VertexLayout, VertexLayoutId, VertexLayoutPred> m_map;

	};

}// namespcae ob