//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphic/MaterialDesc.h>
#include <Framework/Core/Misc/Blob.h>
#include <Framework/RHI/Forward.h>

namespace ob::rhi {
	class CommandList;
}

namespace ob::graphic {

	enum class PropertyType {
		Int,
		Float,
		Vector,
		Color,
		Matrix,
		Texture
	};

	struct ValuePropertyDesc {
		PropertyType type;
		s32 offset = -1;
	};


	class MaterialPass {
	public:

	private:
		Ref<rhi::PipelineState> m_pipeline;
	};


	//@―---------------------------------------------------------------------------
	//! @brief  説明
	//@―---------------------------------------------------------------------------
	class MaterialImpl {
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
		bool hasProprty(StringView name, PropertyType type);

		void setFloat(StringView name, f32 value);
		void setColor(StringView name, Color value);
		void setVector(StringView name, Vec4 value);
		void setMatrix(StringView name, const Matrix& value);
		void setTexture(StringView name, const Ref<Texture>& value);

	public:

		void record(Ref<rhi::CommandList>&,Name pass);

	private:

		template<typename T>
		void setValueProprty(StringView name, PropertyType type, const T& value) {
			if (auto found = m_propertyMap.find(name); found != m_propertyMap.end()) {
				auto& desc = found->second;
				if (desc.type == type)return;
				if (!is_in_range(desc.offset,m_bufferBlob))return;

				auto& dest = *GetOffsetPtr<T>(m_buffer.data(), desc.offset);
				dest = value;
			}
		}

	private:


		Map<String, ValuePropertyDesc, std::less<>> m_propertyMap;
		HashMap<Name, Ref<rhi::PipelineState>> m_passMap;

		Blob m_bufferBlob;
		Array<Ref<Texture>> m_textures;

		Ref<rhi::Buffer> m_buffer;

		Ref<rhi::DescriptorTable> m_bufferTable;
		Ref<rhi::DescriptorTable> m_textureTable;
		Ref<rhi::DescriptorTable> m_samplerTable;

	};

}// namespcae ob