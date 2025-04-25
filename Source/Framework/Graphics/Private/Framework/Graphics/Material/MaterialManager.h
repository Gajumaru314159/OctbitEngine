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

#include <Framework/RHI/RootSignature.h>

namespace ob::graphics {

	//! @brief  説明
	class MaterialManager : public Singleton<MaterialManager> {
	public:

		MaterialManager(rhi::RHI&, NameDictionary&);
		~MaterialManager();

		//!	@brief			レイアウトID取得
		VertexLayoutId getVertexLayoutId(const rhi::VertexLayout& layout);

	public:


		//! @brief  説明

		bool hasProprty(StringView name, MaterialPropertyType type)const;

		bool hasInt(StringView name)const { return hasProprty(name, MaterialPropertyType::Integer); }
		bool hasFloat(StringView name)const { return hasProprty(name, MaterialPropertyType::Scalar); }
		bool hasColor(StringView name)const { return hasProprty(name, MaterialPropertyType::Vector); }
		bool hasMatrix(StringView name)const { return hasProprty(name, MaterialPropertyType::Matrix); }
		bool hasTexture(StringView name)const { return hasProprty(name, MaterialPropertyType::Texture); }

		void setFloat(StringView name, f32 value);
		void setColor(StringView name, Color value);
		void setMatrix(StringView name, const Matrix& value);
		void setTexture(StringView name, const Ref<rhi::Texture>& value);

		auto getSignature() { return m_signature; }

	public:

		void initializeGlobalProperties();
		void initializeRootSignature();

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

		Ref<rhi::RootSignature>		m_signature;

		MemoryStorage<MaterialBlock>	m_block;

	};

}