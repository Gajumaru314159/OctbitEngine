//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphics/Forward.h>
#include <Framework/RHI/Forward.h>
#include <Framework/Graphics/Material/MaterialBlock.h>
#include <Framework/Core/Thread/SpinLock.h>
#include <Framework/Core/Template/Container/Map.h>
#include <functional>

namespace ob::graphics {

	//! @brief  マテリアルシェーダー
	//! @details パス定義やルートシグネチャ、パイプラインキャッシュなどマテリアル間で共有できる情報を保持します。
	class MaterialShader : public RefObject
	{
	public:
		using PipelineState = ob::rhi::PipelineState;
		using RootSignature = ob::rhi::RootSignature;
	public:
		//! @brief  生成
		static Ref<MaterialShader> Create(const MaterialDesc& desc);

		const MaterialDesc& getDesc()const { return m_desc; }
		const MaterialBlockDesc& getBlockDesc()const { return m_blockDesc; }
		const Ref<RootSignature>& getRootSignature()const { return m_signature; }

		s32 calcQualityIndex(StringView pass, s32 quality) const;
		bool prepare(const Ref<Mesh>& mesh);
		Ref<PipelineState> getPipeline(const Ref<Mesh>& mesh, StringView pass, s32 quality);

	private:
		MaterialShader(const MaterialDesc& desc);

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
		using PipelineMap = HashMap<PipelineKey, Ref<PipelineState>, PipelineKeyHasher>;

		Ref<PipelineState> createPipeline(const PipelineKey& key, const ShaderSet& shaders, const rhi::VertexLayout& vertexLayout);

	private:
		MaterialDesc		m_desc;
		MaterialBlockDesc	m_blockDesc;
		Ref<RootSignature>  m_signature;

		SpinLock		    m_pipelinesLock;
		PipelineMap	        m_pipelines;

	};

}
