//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphics/Material/Material.h>
#include <Framework/Graphics/Material/MaterialBlock.h>
#include <Framework/Graphics/Material/MaterialInternalTypes.h>
#include <Framework/Core/Misc/Blob.h>
#include <Framework/RHI/Forward.h>

namespace ob::rhi {
	class CommandList;
}

namespace ob::graphics {

	class Mesh;


	//! @brief  頂点属性
	//! @see    VertexLayout
	struct VertexAttributeKey {
		rhi::Semantic		semantic;	//!< 0x000F
		rhi::ElementType	type;		//!< 0x00F0
		s32					dimention;	//!< 0x0F00
		s32					index;		//!< 0xF000
	};


	struct PipelineKey {
		StringView pass;
		VertexLayoutId layout;

		bool operator==(const PipelineKey& rhs)const {
			return pass == rhs.pass && layout == rhs.layout;
		}
		bool operator!=(const PipelineKey& rhs)const {
			return !(*this==rhs);
		}
	};

	//! @brief  説明
	class MaterialImpl :public Material {
	private:
		using Texture = rhi::Texture;
		using Buffer = rhi::Buffer;
		using RootSignature = rhi::RootSignature;
	public:

		//===============================================================
		// コンストラクタ / デストラクタ
		//===============================================================
		MaterialImpl(const MaterialDesc& desc);

		const MaterialDesc& getDesc()const;

		//! @brief  説明

		bool hasProprty(StringView name, MaterialPropertyType type)const;

		bool hasInt(StringView name)const override { return hasProprty(name, MaterialPropertyType::Integer); }
		bool hasFloat(StringView name)const override { return hasProprty(name, MaterialPropertyType::Scalar); }
		bool hasColor(StringView name)const override { return hasProprty(name, MaterialPropertyType::Vector); }
		bool hasMatrix(StringView name)const override { return hasProprty(name, MaterialPropertyType::Matrix); }
		bool hasTexture(StringView name)const override { return hasProprty(name, MaterialPropertyType::Texture); }
		bool hasBuffer(StringView name)const override { return hasProprty(name, MaterialPropertyType::Buffer); }

		void setFloat(StringView name, f32 value) override;
		void setColor(StringView name, Color value) override;
		void setMatrix(StringView name, const Matrix& value) override;
		void setTexture(StringView name, const Ref<Texture>& value) override;
		void setBuffer(StringView name, const Ref<Buffer>& value) override;

		//! @brief  GPUリソースの事前生成
		bool reserve(const Ref<Mesh>& mesh);

	public:

		void record(Ref<rhi::CommandList>&, const Matrix&, const Ref<Mesh>& mesh, s32 submesh, StringView pass);
		void record(Ref<rhi::CommandList>&, Span<Matrix>, const Ref<Mesh>& mesh, s32 submesh, StringView pass);

		Ref<rhi::PipelineState> createPipeline(StringView pass,const rhi::VertexLayout& layout, VertexLayoutId id);

	private:

		struct PipelineKeyHasher {
		public:
			size_t operator()(const ob::graphics::PipelineKey& v)const {
				return std::hash<decltype(v.pass)>{}(v.pass) ^ std::hash<decltype(v.layout)>{}(v.layout);
			}
		};

		using PipelineMap = HashMap<PipelineKey, Ref<rhi::PipelineState>, PipelineKeyHasher>;

	private:

		const MaterialDesc	m_desc;

		SpinLock			m_lock;

		PipelineMap			m_pipelineMap;

		Ref<rhi::DescriptorLayout> m_materialLayout;


		MaterialBlock       m_block;

		Ref<RootSignature>  m_signature;


	};

}