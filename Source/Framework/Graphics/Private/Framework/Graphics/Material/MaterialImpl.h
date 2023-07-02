//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphics/Material.h>
#include <Framework/Graphics/Material/MaterialInternalTypes.h>
#include <Framework/Core/Misc/Blob.h>
#include <Framework/RHI/Forward.h>

namespace ob::rhi {
	class CommandList;
}

namespace ob::graphics {

	class Mesh;


	//@―---------------------------------------------------------------------------
	//! @brief  頂点属性
	//! @see    VertexLayout
	//@―---------------------------------------------------------------------------
	struct VertexAttributeKey {
		rhi::Semantic	semantic;	//!< 0x000F
		rhi::Type		type;		//!< 0x00F0
		s32				dimention;	//!< 0x0F00
		s32				index;		//!< 0xF000
	};


	struct PipelineKey {
		Name pass;
		VertexLayoutId layout;

		bool operator==(const PipelineKey& rhs)const {
			return pass == rhs.pass && layout == rhs.layout;
		}
		bool operator!=(const PipelineKey& rhs)const {
			return !(*this==rhs);
		}
	};

	//@―---------------------------------------------------------------------------
	//! @brief  説明
	//@―---------------------------------------------------------------------------
	class MaterialImpl :public Material {
	private:
		using Texture = rhi::Texture;

	public:

		//===============================================================
		// コンストラクタ / デストラクタ
		//===============================================================
		MaterialImpl(const MaterialDesc& desc);

		//@―---------------------------------------------------------------------------
		//! @brief  説明
		//@―---------------------------------------------------------------------------

		bool hasProprty(StringView name, PropertyType type)const;

		bool hasInt(StringView name)const override { return hasProprty(name, PropertyType::Int); }
		bool hasFloat(StringView name)const override { return hasProprty(name, PropertyType::Float); }
		bool hasColor(StringView name)const override { return hasProprty(name, PropertyType::Color); }
		bool hasMatrix(StringView name)const override { return hasProprty(name, PropertyType::Matrix); }
		bool hasTexture(StringView name)const override { return hasProprty(name, PropertyType::Texture); }

		void setFloat(StringView name, f32 value) override;
		void setColor(StringView name, Color value) override;
		void setMatrix(StringView name, const Matrix& value) override;
		void setTexture(StringView name, const Ref<Texture>& value) override;

		//@―---------------------------------------------------------------------------
		//! @brief  GPUリソースの事前生成
		//@―---------------------------------------------------------------------------
		bool reserve(const Ref<Mesh>& mesh);

	public:

		void record(Ref<rhi::CommandList>&, const Matrix&, const Ref<Mesh>& mesh, s32 submesh, Name pass);
		void record(Ref<rhi::CommandList>&, Span<Matrix>, const Ref<Mesh>& mesh, s32 submesh, Name pass);

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

		Ref<rhi::PipelineState> createPipeline(Name pass,const rhi::VertexLayout& layout, VertexLayoutId id);

	private:

		struct PipelineKeyHasher {
		public:
			size_t operator()(const ob::graphics::PipelineKey& v)const {
				return std::hash<decltype(v.pass)>{}(v.pass) ^ std::hash<decltype(v.layout)>{}(v.layout);
			}
		};

		using PipelineMap = HashMap<PipelineKey, Ref<rhi::PipelineState>, PipelineKeyHasher>;
		using PropertyMap = Map<String, ValuePropertyDesc, std::less<>>;

		const MaterialDesc	m_desc;

		PipelineMap			m_pipelineMap;
		PropertyMap			m_propertyMap;

		Blob				m_bufferBlob;

		Ref<rhi::Buffer>	m_buffer;
		Array<Ref<Texture>> m_textures;

		Ref<rhi::DescriptorTable> m_bufferTable;
		Ref<rhi::DescriptorTable> m_textureTable;
		Ref<rhi::DescriptorTable> m_samplerTable;

	};

}