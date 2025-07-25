//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphics/Forward.h>
#include <Framework/Graphics/Material/MaterialDesc.h>
#include <Framework/Graphics/Material/MaterialBlock.h>
#include <Framework/Graphics/Material/MaterialSystem.h>

namespace ob::graphics {

	//! @brief  マテリアル
	class Material : public RefObject {
	public:
		using Texture = ob::rhi::Texture;
		using Sampler = ob::rhi::Sampler;
		using Buffer = ob::rhi::Buffer;
	public:

		//! @brief  生成
		static Ref<Material> Create(const MaterialDesc& desc);

	public:

		virtual const MaterialDesc& getDesc()const = 0;

		//! @brief  マテリアルパラメータが存在するか
		virtual bool hasProperty(StringView name, MaterialPropertyType type)const = 0;
		bool hasTexture(StringView name)const;  //!< @copybrief hasProperty()
		bool hasBuffer(StringView name)const;   //!< @copybrief hasProperty()
		bool hasMatrix(StringView name)const;   //!< @copybrief hasProperty()
		bool hasVector(StringView name)const;   //!< @copybrief hasProperty()
		bool hasScalar(StringView name)const;   //!< @copybrief hasProperty()
		bool hasInteger(StringView name)const;  //!< @copybrief hasProperty()

		//! @brief  マテリアルパラメータを設定
		virtual void setTexture(StringView name, const Ref<Texture>& value) = 0;
		virtual void setBuffer(StringView name, const Ref<Buffer>& value) = 0;
		virtual void setMatrix(StringView name, const Matrix& value) = 0;
		virtual void setVector(StringView name, Color value) = 0;
		virtual void setScalar(StringView name, f32 value) = 0;
		virtual void setInteger(StringView name, f32 value) = 0;

		virtual s32 calcQualityIndex(StringView pass, s32 quality) const = 0;

		//! @brief  GPUリソースの事前生成
		//! @details GPUリソースを事前生成しておくことで描画時のスパイクを回避することができます。
		virtual bool prepare(const Ref<Mesh>& mesh) = 0;

		virtual void record(Ref<rhi::CommandList>& commandList, MaterialBlockSet& blocks, const Ref<Mesh>& mesh, s32 submesh, StringView pass, s32 quality = 0) = 0;

	public:

		//! @brief  グローバルマテリアルパラメータを設定
		//! @{
		static void SetGlobalTexture(StringView name, const Ref<Texture>& value);
		static void SetGlobalBuffer(StringView name, const Ref<Buffer>& value);
		static void SetGlobalMatrix(StringView name, const Matrix& value);
		static void SetGlobalVector(StringView name, Color value);
		static void SetGlobalScalar(StringView name, f32 value);
		static void SetGlobalInteger(StringView name, s32 value);
		//! @}

	};



	inline bool Material::hasTexture(StringView name)const { return hasProperty(name, MaterialPropertyType::Texture); }
	inline bool Material::hasBuffer(StringView name)const { return hasProperty(name, MaterialPropertyType::Buffer); }
	inline bool Material::hasMatrix(StringView name)const { return hasProperty(name, MaterialPropertyType::Matrix); }
	inline bool Material::hasVector(StringView name)const { return hasProperty(name, MaterialPropertyType::Vector); }
	inline bool Material::hasScalar(StringView name)const { return hasProperty(name, MaterialPropertyType::Scalar); }
	inline bool Material::hasInteger(StringView name)const { return hasProperty(name, MaterialPropertyType::Integer); }

}