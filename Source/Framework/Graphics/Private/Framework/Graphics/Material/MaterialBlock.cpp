//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphics/Material/MaterialBlock.h>
#include <Framework/RHI/RHI.h>
#include <Framework/RHI/Buffer.h>
#include <Framework/RHI/Texture.h>
#include <Framework/RHI/Sampler.h>
#include <Framework/RHI/CommandList.h>
#include <Framework/RHI/DescriptorTable.h>

namespace ob::graphics {

	enum ResourceHandleType {
		Unknown,
		Texture,
		Sampler,
		Buffer,
	};

	struct alignas(16) BufferHandle {
		u32 index;
		ResourceHandleType type = ResourceHandleType::Buffer;
		u32 padding[2] = { 0,0 };

		bool operator==(const BufferHandle& rhs)const { return index == rhs.index; }
	};
	struct alignas(16) TextureHandle {
		u32 index;
		ResourceHandleType type = ResourceHandleType::Texture;
		u32 padding[2] = { 0,0 };

		bool operator==(const TextureHandle& rhs)const { return index == rhs.index; }
	};
	struct alignas(16) SamplerHandle {
		u32 index;
		ResourceHandleType type = ResourceHandleType::Sampler;
		u32 padding[2] = { 0,0 };

		bool operator==(const SamplerHandle& rhs)const { return index == rhs.index; }
	};
	struct TextureAndSamplerHandle {
		TextureHandle texture;
		SamplerHandle sampler;

		bool operator==(const TextureAndSamplerHandle& rhs)const { return texture == rhs.texture && sampler == rhs.sampler; }
	};

	//! @brief コンストラクタ
	//! @param desc マテリアルブロックの説明
	MaterialBlock::MaterialBlock(const MaterialBlockDesc& desc) {
		initializeProperties(desc);
		initializeDescriptorTables();
	}

	//! @brief プロパティを初期化する
	//! @param desc マテリアルブロックの説明
	void MaterialBlock::initializeProperties(const MaterialBlockDesc& desc) {
		using namespace ob::rhi;

		m_isBindless = RHI::Instance().getConfig().enableBindless;

		// NOTE ここで生成しているマップはMaterialBlockDescが同じであればシステム内で共有可能
		//      キャッシュ対応することによってメモリ消費量の削減が見込める

		// 定数変数のパッキング 規則 (https://learn.microsoft.com/ja-jp/windows/win32/direct3dhlsl/dx-graphics-hlsl-packing-rules)
		// * Integer 4 byte aligned
		// * Scalar  4 byte aligned
		// * Vector  16 byte aligned
		// * Matrix  16 byte aligned

		// プロパティ名とオフセットを対応 (パディングが入らないようにアライメントが大きいものから)
		s32 offset = 0;

		if (m_isBindless) {

			// バインドレス時はパラメーターバッファ内にハンドルを格納
			for (auto [index, name] : Indexed(desc.textures)) {
				auto [itr, added] = m_properties.try_emplace(name, MaterialValuePropertyDesc{ MaterialPropertyType::Texture,offset,(s32)index });
				if (!added) LOG_ERROR("プロパティ[{}]はマテリアルに既に含まれています。", name);
				offset += sizeof(TextureAndSamplerHandle);
			}
			m_textures.resize(desc.textures.size());
			m_samplers.resize(desc.textures.size());

			for (auto [index, name] : Indexed(desc.buffers)) {
				auto [itr, added] = m_properties.try_emplace(name, MaterialValuePropertyDesc{ MaterialPropertyType::Buffer,offset,(s32)index });
				if (!added) LOG_ERROR("プロパティ[{}]はマテリアルに既に含まれています。", name);
				offset += sizeof(BufferHandle);
			}
			m_buffers.resize(desc.buffers.size());
		} else {

			// バインドフル時は専用のDescriptorTableにリソースを格納
			for (auto [index, name] : Indexed(desc.textures)) {
				auto [itr, added] = m_properties.try_emplace(name, MaterialValuePropertyDesc{ MaterialPropertyType::Texture,-1,(s32)index });
				if (!added) LOG_ERROR("プロパティ[{}]はマテリアルに既に含まれています。", name);
			}
			m_textures.resize(desc.textures.size());
			m_samplers.resize(desc.textures.size());

			for (auto [index, name] : Indexed(desc.buffers)) {
				auto [itr, added] = m_properties.try_emplace(name, MaterialValuePropertyDesc{ MaterialPropertyType::Buffer,-1,(s32)index });
				if (!added) LOG_ERROR("プロパティ[{}]はマテリアルに既に含まれています。", name);
			}
			m_buffers.resize(desc.buffers.size());

		}

		for (auto& name : desc.matrices) {
			auto [itr, added] = m_properties.try_emplace(name, MaterialValuePropertyDesc{ MaterialPropertyType::Matrix,offset });
			if (!added) LOG_ERROR("プロパティ[{}]はマテリアルに既に含まれています。", name);
			offset += sizeof(Matrix);
		}

		for (auto& name : desc.vectors) {
			auto [itr, added] = m_properties.try_emplace(name, MaterialValuePropertyDesc{ MaterialPropertyType::Vector,offset });
			if (!added) LOG_ERROR("プロパティ[{}]はマテリアルに既に含まれています。", name);
			offset += sizeof(Vec4);
		}

		for (auto& name : desc.scalars) {
			auto [itr, added] = m_properties.try_emplace(name, MaterialValuePropertyDesc{ MaterialPropertyType::Scalar,offset });
			if (!added) LOG_ERROR("プロパティ[{}]はマテリアルに既に含まれています。", name);
			offset += sizeof(f32);
		}

		for (auto& name : desc.integers) {
			auto [itr, added] = m_properties.try_emplace(name, MaterialValuePropertyDesc{ MaterialPropertyType::Integer,offset });
			if (!added) LOG_ERROR("プロパティ[{}]はマテリアルに既に含まれています。", name);
			offset += sizeof(s32);
		}

		// バッファ生成
		size_t size = offset;
		auto bufferDesc = rhi::BufferDesc::Constant(size);
		bufferDesc.name = Format("MaterialParameter ({})", desc.name);
		m_valuesBuffer = rhi::Buffer::Create(bufferDesc);
		OB_ASSERT_EXPR(m_valuesBuffer);

		m_values.resize(size);
		m_valuesDebug = Span<Component>(reinterpret_cast<Component*>(m_values.data()), m_values.size() / sizeof(Component));

	}

	//! @brief デスクリプタテーブルを初期化する
	//! @param desc マテリアルブロックの説明
	void MaterialBlock::initializeDescriptorTables() {
		using namespace ob::rhi;
		size_t srvNum = m_textures.size() + m_buffers.size() + 1;
		size_t uavNum = 0;
		size_t samplerNum = m_textures.size();

		// m_tableCBV Bindless時はパラメーターはByteAddressBufferで渡されるのでSRVを使用する
		m_tableSRV = DescriptorTable::Create(DescriptorRangeType::SRV, srvNum);
		m_tableUAV = DescriptorTable::Create(DescriptorRangeType::UAV, uavNum);
		m_tableSampler = DescriptorTable::Create(DescriptorRangeType::Sampler, samplerNum);

		s32 srv = 0;

		for (s32 i = 0; i < m_textures.size(); ++i) {
			m_tableSRV->setResource(srv++, Texture::White());
			m_tableSampler->setResource(i, Sampler::Default());
		}
		for (s32 i = 0; i < m_buffers.size(); ++i) {
			//m_tableSRV->setResource(srv++, Buffer::Empty());
			srv++;
		}
		{
			m_tableSRV->setResource(srv++, m_valuesBuffer);
		}
	}

	//! @brief  プロパティがあるか
	bool MaterialBlock::hasProprty(StringView name, MaterialPropertyType type) const {
		if (auto found = m_properties.find(name); found != m_properties.end()) {
			return found->second.type == type;
		}
		return false;
	}

	//! @brief パラメーターバッファに対してプロパティを設定する
	//! @tparam T 設定する値の型
	//! @tparam TEq 設定する値の比較オブジェクト形
	//! @param name 
	//! @param type 
	//! @param value 
	template<typename T, typename TEq>
	void MaterialBlock::setValueProprty(StringView name, MaterialPropertyType type, const T& value) {
		if (auto found = m_properties.find(name); found != m_properties.end()) {
			auto& desc = found->second;
			if (desc.type != type)return;
			if (!is_in_range(desc.offset + sizeof(T) - 1, m_values))return;

			auto& dest = *GetOffsetPtr<T>(m_values.data(), desc.offset);

			if (TEq()(value, dest))return;

			dest = value;

			m_hasChanged = true;
		}
	}

	//! @brief  Floatプロパティを設定
	void MaterialBlock::setInteger(StringView name, s32 value) {
		setValueProprty(name, MaterialPropertyType::Scalar, value);
	}

	//! @brief  Floatプロパティを設定
	void MaterialBlock::setScalar(StringView name, f32 value) {
		setValueProprty(name, MaterialPropertyType::Scalar, value);
	}

	//! @brief  Colorプロパティを設定
	void MaterialBlock::setVector(StringView name, Color value) {
		setValueProprty(name, MaterialPropertyType::Vector, value);
	}

	//! @brief  Matrixプロパティを設定
	void MaterialBlock::setMatrix(StringView name, const Matrix& value) {
		setValueProprty(name, MaterialPropertyType::Matrix,
#if 1
			value
#else
			value.transposed()
#endif
		);
	}

	//! @brief  Textureプロパティを設定
	void MaterialBlock::setTexture(StringView name, const Ref<Texture>& texture, const Ref<Sampler>& sampler) {

		if (!texture) {
			LOG_ERROR("プロパティ[{}]に空のテクスチャを設定しようとしました", name);
			return;
		}
		if (!sampler) {
			LOG_ERROR("プロパティ[{}]に空のサンプラーを設定しようとしました", name);
			return;
		}

		bool useBindless = rhi::RHI::Instance().getConfig().enableBindless;

		if (auto found = m_properties.find(name); found != m_properties.end()) {

			auto& desc = found->second;
			
			// バリデート
			if (desc.type != MaterialPropertyType::Texture)return;
			if (useBindless) {
				if (!is_in_range(desc.offset + sizeof(TextureAndSamplerHandle) - 1, m_values))return;
			} else {
				if (!is_in_range(desc.index, m_textures))return;
			}

			// TODO Bindless時もリロード用に保持する
			// NOTE リロードイベントはテクスチャの中に持たせないほうがいいかも...
			//      Notifierのサイズが大きくなる。
			m_textures[desc.index] = texture;
			m_samplers[desc.index] = sampler;

			m_tableSRV->setResource(desc.index, texture);
			m_tableSampler->setResource(desc.index, sampler);

			if (useBindless) {
				TextureAndSamplerHandle handles;
				handles.texture.index = m_tableSRV->getBindlessIndex(desc.index);
				handles.sampler.index = m_tableSampler->getBindlessIndex(desc.index);
				setValueProprty(name, MaterialPropertyType::Texture, handles);
			}

		}
	}

	//! @brief  Bufferプロパティを設定
	void MaterialBlock::setBuffer(StringView name, const Ref<rhi::Buffer>& value) {

		bool useBindless = rhi::RHI::Instance().getConfig().enableBindless;

		if (auto found = m_properties.find(name); found != m_properties.end()) {

			auto& desc = found->second;

			// バリデート
			if (desc.type != MaterialPropertyType::Buffer)return;
			if (useBindless) {
				if (!is_in_range(desc.offset + sizeof(BufferHandle) - 1, m_values))return;
			} else {
				if (!is_in_range(desc.index, m_buffers))return;
			}

			m_buffers[desc.index] = value;

			m_tableSRV->setResource(m_textures.size() + desc.index, value);

			if (useBindless) {
				BufferHandle handle;
				handle.index = m_tableSRV->getBindlessIndex(m_textures.size() + desc.index);
				setValueProprty(name, MaterialPropertyType::Buffer, handle);
			}

		}
	}


	//! @brief MaterialBlockのハンドルを指定のスロットに記録する
	//!@details Bindfull時のみ使用可能です。
	//!         この関数を呼び出すと、指定のスロットに対してDescriptorTableが設定されます。
	//!         Slotに-1が指定された場合、そのスロットには記録されません。
	//!         CBVの先頭にはパラメータのバッファが記録され、その後ろにBufferのリストが記録されます。
	//!         詳細は単体テストを参照してください。
	void MaterialBlock::record(Ref<CommandList>& commandList, s32 srvSlot, s32 uavSlot, s32 samplerSlot) {
		if (!commandList) return;
		using namespace ob::rhi;

		if (m_isBindless) return;

		updateParameterBuffer();

		FixedVector < SetDescriptorTableParam, 3> params;
		if (0 <= srvSlot) {
			auto& param = params.emplace_back();
			param.slot = srvSlot;
			param.table = m_tableSRV;
		}
		if (0 <= uavSlot) {
			auto& param = params.emplace_back();
			param.slot = uavSlot;
			param.table = m_tableUAV;
		}
		if (0 <= samplerSlot) {
			auto& param = params.emplace_back();
			param.slot = samplerSlot;
			param.table = m_tableSampler;
		}

		commandList->setRootDesciptorTable(params.data(), params.size());

	}

	//! @brief MaterialBlockのハンドルを指定のスロットに記録する
	//! @details Bindless時のみ使用可能です。
	//!          この関数を呼び出すと、指定のスロットに対してMaterialBlockのBufferHandle記録されます。
	void MaterialBlock::record(Ref<CommandList>& commandList, s32 slot) {
		if (!commandList) return;
		using namespace ob::rhi;

		if (!m_isBindless) return;

		updateParameterBuffer();

		size_t valuesIndex = m_textures.size() + m_buffers.size();

		BufferHandle handle;
		handle.index = m_tableSRV->getBindlessIndex(valuesIndex);

		SetRootConstantsParam param;
		param.slot = slot;
		param.blob = BlobView(&handle, sizeof(handle));
		param.offset = 0;
		commandList->setRootConstant(param);

	}

	//! @brief パラメーターバッファを更新する
	void MaterialBlock::updateParameterBuffer() {
		if (m_hasChanged) {
			m_valuesBuffer->updateDirect(m_values.size(), m_values.data());
			m_hasChanged = false;
		}
	}

}