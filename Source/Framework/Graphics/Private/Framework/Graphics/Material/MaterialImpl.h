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

#include <Framework/RHI/Sampler.h>

namespace ob::rhi {
	class CommandList;
}

namespace ob::graphics {

	class MaterialImpl : public Material {
	public:
		using PipelineState = ob::rhi::PipelineState;
		using RootSignature = ob::rhi::RootSignature;
	public:
		MaterialImpl(const MaterialDesc& desc);

		const MaterialDesc& getDesc()const override { return m_desc; }

		//! @brief  マテリアルパラメータが存在するか
		bool hasProperty(StringView name, MaterialPropertyType type)const override { return m_block.hasProperty(name,type); }

		//! @brief  マテリアルパラメータを設定
		void setTexture(StringView name, const Ref<Texture>& value) { m_block.setTexture(name,value,rhi::Sampler::Default()); }
		void setBuffer(StringView name, const Ref<Buffer>& value) { m_block.setBuffer(name,value); }
		void setMatrix(StringView name, const Matrix& value) { m_block.setMatrix(name,value); }
		void setVector(StringView name, Color value) { m_block.setVector(name,value); }
		void setScalar(StringView name, f32 value) { m_block.setScalar(name,value); }
		void setInteger(StringView name, f32 value) { m_block.setInteger(name,value); }
		
		//! @brief Meshの描画に必要なリソースを事前確保する
		bool prepare(const Ref<Mesh>& mesh) override;

		s32 calcQualityIndex(StringView pass, s32 quality) const override;

		void record(Ref<rhi::CommandList>& commandList, MaterialBlockSet& blocks, const Ref<Mesh>& mesh, s32 submesh, StringView pass, s32 quality) override;

	private:
		struct PipelineKey {
			String pass;
			s32 qualityIndex;
			VertexLayoutId layout;
			bool operator==(const PipelineKey& rhs)const { return pass == rhs.pass && qualityIndex == rhs.qualityIndex && layout == rhs.layout; }
			bool operator!=(const PipelineKey& rhs)const { return !(*this == rhs); }
		};
		struct PipelineKeyHasher {
			size_t operator()(const PipelineKey& v)const { return std::hash<decltype(v.pass)>{}(v.pass) ^ std::hash<decltype(v.qualityIndex)>{}(v.qualityIndex) ^ std::hash<decltype(v.layout)>{}(v.layout); }
		};
		using PipelineMap = HashMap<PipelineKey, Ref<rhi::PipelineState>, PipelineKeyHasher>;
	private:

		Ref<PipelineState> createPipeline(const PipelineKey& key, const ShaderSet& shaders, const rhi::VertexLayout& vertexLayout);

	private:
		MaterialDesc		m_desc;			//!< 描画パスを含むマテリアルの説明
		Ref<RootSignature>  m_signature;	//!< MaterialDesc に対応する RootSignature
		MaterialBlock       m_block;		//!< Material 毎のプロパティブロック

		SpinLock		    m_pipelinesLock;
		PipelineMap	        m_pipelines;

	};

}