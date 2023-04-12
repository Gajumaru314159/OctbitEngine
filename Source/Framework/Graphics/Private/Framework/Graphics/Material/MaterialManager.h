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
#include <Framework/Graphics/Material/MaterialInternalTypes.h>

namespace ob::graphics {

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

	public:


		//@―---------------------------------------------------------------------------
		//! @brief  説明
		//@―---------------------------------------------------------------------------

		bool hasProprty(StringView name, PropertyType type)const;

		bool hasInt(StringView name)const { return hasProprty(name, PropertyType::Int); }
		bool hasFloat(StringView name)const { return hasProprty(name, PropertyType::Float); }
		bool hasColor(StringView name)const { return hasProprty(name, PropertyType::Color); }
		bool hasMatrix(StringView name)const { return hasProprty(name, PropertyType::Matrix); }
		bool hasTexture(StringView name)const { return hasProprty(name, PropertyType::Texture); }

		void setFloat(StringView name, f32 value);
		void setColor(StringView name, Color value);
		void setMatrix(StringView name, const Matrix& value);
		void setTexture(StringView name, const Ref<rhi::Texture>& value);

	private:

		template<typename T, typename TEq = std::equal_to<T>>
		void setValueProprty(StringView name, PropertyType type, const T& value) {
			if (auto found = m_propertyMap.find(name); found != m_propertyMap.end()) {
				auto& desc = found->second;
				if (desc.type != type)return;
				if (!is_in_range(desc.offset, m_bufferBlob))return;

				auto& dest = *GetOffsetPtr<T>(m_bufferBlob.data(), desc.offset);

				if (TEq()(value, dest))return;

				dest = value;
			}
		}
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

		HashMap<engine::Name, rhi::SubPass> m_renderPassMap;

		SpinLock m_lock;
		Map<rhi::VertexLayout, VertexLayoutId, VertexLayoutPred> m_map;


		PropertyMap					m_propertyMap;

		Blob						m_bufferBlob;

		Ref<rhi::Buffer>			m_buffer;
		Array<Ref<rhi::Texture>>	m_textures;

		Ref<rhi::DescriptorTable>	m_bufferTable;
		Ref<rhi::DescriptorTable>	m_textureTable;
		Ref<rhi::DescriptorTable>	m_samplerTable;

	};

}