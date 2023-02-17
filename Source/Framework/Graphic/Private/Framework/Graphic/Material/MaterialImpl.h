//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphic/Material.h>
#include <Framework/Core/Misc/Blob.h>
#include <Framework/RHI/Forward.h>
#include <Framework/Engine/Name.h>

namespace ob::rhi {
	class CommandList;
}

namespace ob::graphic {

	class Mesh;

	enum class PropertyType {
		Int,
		Float,
		Color,
		Matrix,
		Texture
	};

	//@―---------------------------------------------------------------------------
	//! @brief  説明
	//@―---------------------------------------------------------------------------
	class MaterialImpl:public Material {
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

		bool hasInt(StringView name)const override{ return hasProprty(name,PropertyType::Int); }
		bool hasFloat(StringView name)const override { return hasProprty(name, PropertyType::Float); }
		bool hasColor(StringView name)const override { return hasProprty(name, PropertyType::Color); }
		bool hasMatrix(StringView name)const override { return hasProprty(name, PropertyType::Matrix); }
		bool hasTexture(StringView name)const override { return hasProprty(name, PropertyType::Texture); }

		void setFloat(StringView name, f32 value) override;
		void setColor(StringView name, Color value) override;
		void setMatrix(StringView name, const Matrix& value) override;
		void setTexture(StringView name, const Ref<Texture>& value) override;

	public:

		void record(Ref<rhi::CommandList>&, const Ref<Mesh>& mesh,engine::Name pass);

	private:

		template<typename T,typename TEq = std::equal_to<T>>
		void setValueProprty(StringView name, PropertyType type, const T& value) {
			if (auto found = m_propertyMap.find(name); found != m_propertyMap.end()) {
				auto& desc = found->second;
				if (desc.type == type)return;
				if (!is_in_range(desc.offset,m_bufferBlob))return;
				if (TEq()(value, dest))return;

				auto& dest = *GetOffsetPtr<T>(m_buffer.data(), desc.offset);
				dest = value;
			}
		}

		void creaePipeline();

	private:

		struct ValuePropertyDesc {
			PropertyType type;
			s32 offset = -1;
		};

		using PassMap = HashMap<engine::Name, Ref<rhi::PipelineState>>;
		using PropertyMap = Map<String, ValuePropertyDesc, std::less<>>;

		MaterialDesc		m_desc;

		PassMap				m_passMap;
		PropertyMap			m_propertyMap;

		Blob				m_bufferBlob;

		Ref<rhi::Buffer>	m_buffer;
		Array<Ref<Texture>> m_textures;

		Ref<rhi::DescriptorTable> m_bufferTable;
		Ref<rhi::DescriptorTable> m_textureTable;
		Ref<rhi::DescriptorTable> m_samplerTable;

	};

}// namespcae ob