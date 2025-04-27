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
#include <Framework/RHI/DescriptorLayout.h>
#include <Framework/RHI/DescriptorTable.h>

namespace ob::graphics {

	//! @brief MaterialBlockDescに対応するDescriptorLayoutを生成するユーティリティ関数
	Ref<rhi::DescriptorLayout> MaterialBlock::CreateLayout(const MaterialBlockDesc& desc) {
		using namespace ob::rhi;

		DescriptorLayoutDesc layoutDesc;
		layoutDesc.name = desc.name;

		s32 index = 0;
		for (auto& name : desc.textures) {
			layoutDesc.items.emplace_back(Binding::Texture(index++));
			layoutDesc.items.emplace_back(Binding::Sampler(index++));
		}
		for (auto& name : desc.buffers) {
			layoutDesc.items.emplace_back(Binding::ByteAddressBuffer(index++));
		}
		{
			layoutDesc.items.emplace_back(Binding::ByteAddressBuffer(index++));
		}

		return DescriptorLayout::Create(layoutDesc);
	}

	//! @brief コンストラクタ
	//! @param desc マテリアルブロックの説明
	MaterialBlock::MaterialBlock(const MaterialBlockDesc& desc) {
		initializeProperties(desc);
		initializeDescriptorTables(desc);
	}


	//! @brief プロパティを初期化する
	//! @param desc マテリアルブロックの説明
	void MaterialBlock::initializeProperties(const MaterialBlockDesc& desc) {
		using namespace ob::rhi;

		m_isBindless = RHI::Instance().getConfig().enableBindless;

		// NOTE ここで生成しているマップはMaterialBlockDescが同じであればシステム内で共有可能
		//      キャッシュ対応することによってメモリ消費量の削減が見込める

		// 定数変数のパッキング 規則 (https://learn.microsoft.com/ja-jp/windows/win32/direct3dhlsl/dx-graphics-hlsl-packing-rules)
		// * Matrix  16 byte aligned
		// * Vector  16 byte aligned
		// * Scalar  4 byte aligned
		// * Integer 4 byte aligned

		// プロパティ名とオフセットを対応 (パディングが入らないようにアライメントが大きいものから)
		s32 offset = 0;
		s32 slot = 0;

		if (m_isBindless) {

			// バインドレス時はパラメーターバッファ内にハンドルを格納
			for (auto [index, name] : Indexed(desc.textures)) {
				auto [itr, added] = m_properties.try_emplace(name, MaterialValuePropertyDesc{ MaterialPropertyType::Texture,offset,(s32)index,slot});
				if (!added) LOG_ERROR("プロパティ[{}]はマテリアルに既に含まれています。", name);
				offset += sizeof(rhi::BindlessHandle) * 2;
				slot += 2;
			}
			m_textures.resize(desc.textures.size());
			m_samplers.resize(desc.textures.size());

			for (auto [index, name] : Indexed(desc.buffers)) {
				auto [itr, added] = m_properties.try_emplace(name, MaterialValuePropertyDesc{ MaterialPropertyType::Buffer,offset,(s32)index,slot});
				if (!added) LOG_ERROR("プロパティ[{}]はマテリアルに既に含まれています。", name);
				offset += sizeof(rhi::BindlessHandle);
				slot += 1;
			}
			m_buffers.resize(desc.buffers.size());
		} else {

			// バインドフル時は専用のDescriptorTableにリソースを格納
			for (auto [index, name] : Indexed(desc.textures)) {
				auto [itr, added] = m_properties.try_emplace(name, MaterialValuePropertyDesc{ MaterialPropertyType::Texture,-1,(s32)index,slot});
				if (!added) LOG_ERROR("プロパティ[{}]はマテリアルに既に含まれています。", name);
				slot+=2;
			}
			m_textures.resize(desc.textures.size());
			m_samplers.resize(desc.textures.size());

			for (auto [index, name] : Indexed(desc.buffers)) {
				auto [itr, added] = m_properties.try_emplace(name, MaterialValuePropertyDesc{ MaterialPropertyType::Buffer,-1,(s32)index,slot});
				if (!added) LOG_ERROR("プロパティ[{}]はマテリアルに既に含まれています。", name);
				slot++;
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
		size_t size = align_up(offset,16);
		auto bufferDesc = rhi::BufferDesc::ByteAddress(size);
		bufferDesc.name = Format("MaterialParameter ({})", desc.name);
		m_valuesBuffer = rhi::Buffer::Create(bufferDesc);
		OB_ASSERT_EXPR(m_valuesBuffer);

		m_values.resize(size);
#if OB_MATERIAL_BLOCK_DEBUG_ENABLED
		m_valuesDebug = Span<Component>(reinterpret_cast<Component*>(m_values.data()), m_values.size() / sizeof(Component));
#endif
	}


	//! @brief デスクリプタテーブルを初期化する
	//! @param desc マテリアルブロックの説明
	void MaterialBlock::initializeDescriptorTables(const MaterialBlockDesc& desc) {
		using namespace ob::rhi;

		// バリデート (バインドレスはルート定数を使うのでバインドフルのみ)
		if(desc.layout) {
			s32 index = 0;
			auto& items = desc.layout->getDesc().items;;
			for (auto& name : desc.textures) {
				if (items.at(index++).type != BindingType::Texture) {
					LOG_ERROR("DescriptorLayoutとパラメータが一致していません");
					return;
				}
				if (items.at(index++).type != BindingType::Sampler) {
					LOG_ERROR("DescriptorLayoutとパラメータが一致していません");
					return;
				}
			}
			for (auto& name : desc.buffers) {
				if (items.at(index++).type != BindingType::ByteAddressBuffer) {
					LOG_ERROR("DescriptorLayoutとパラメータが一致していません");
					return;
				}
			}
			{
				if (items.at(index++).type != BindingType::ByteAddressBuffer) {
					LOG_ERROR("DescriptorLayoutとパラメータが一致していません");
					return;
				}
			}
		}
			

		// https://github.com/sebbbi/perftest
		// によるとConstantBufferでもByteAddressBufferでも同じ速度が出るらしい
		// なのでMaterialBlockではCBV(ConstantBuffer)は使用せず、SRV(ByteAddressBuffer/StructuredBuffer)を使用する。
		// 唯一の例外はBindless時にValuesBufferのBufferHandleをRootConstantで渡すとき。
		// C++側はRootSignatureでConstantを使用し、シェーダーではb(CBV)を使用する。

		if (desc.layout) {
			m_table = DescriptorTable::Create({ desc.layout });
		} else {
			// バインドレスの場合はDescriptorTableは使用しないが、BindlessHandleを取得するためにDescriptorLayoutを作成する
			s32 index = 0;
			DescriptorLayoutDesc layoutDesc;
			for (auto& texture : desc.textures) {
				layoutDesc.items.push_back(Binding::Texture(index++));
				layoutDesc.items.push_back(Binding::Sampler(index++));
			}
			for (auto& texture : desc.textures) {
				layoutDesc.items.push_back(Binding::ByteAddressBuffer(index++));
			}
			{
				layoutDesc.items.push_back(Binding::ByteAddressBuffer(index++));
			}
			auto layout = DescriptorLayout::Create(layoutDesc);
			m_table = DescriptorTable::Create({ layout });
		}


		s32 index = 0;

		for (s32 i = 0; i < m_textures.size(); ++i) {
			m_table->setResource(index++, Texture::White());
			m_table->setResource(index++, Sampler::Default());
		}
		for (s32 i = 0; i < m_buffers.size(); ++i) {
			//m_tableSRV->setResource(srv++, Buffer::Empty());
			index++;
		}
		{
			m_table->setResource(index++, m_valuesBuffer);
		}
	}


	//! @brief  プロパティがあるか
	bool MaterialBlock::hasProperty(StringView name, MaterialPropertyType type) const {
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

		if (!m_table) {
			LOG_ERROR("MaterialBlockの構築に失敗しています [{}]",name);
			return;
		}
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
				if (!is_in_range(desc.offset + sizeof(rhi::BindlessHandle) * 2 - 1, m_values))return;
			} else {
				if (!is_in_range(desc.index, m_textures))return;
			}

			m_textures[desc.index] = texture;
			m_samplers[desc.index] = sampler;

			m_table->setResource(desc.slot+0, texture);
			m_table->setResource(desc.slot+1, sampler);

			if (useBindless) {

				struct TextureSamplerHandle {
					rhi::BindlessHandle texture;
					rhi::BindlessHandle sampler;
					bool operator == (const TextureSamplerHandle& rhs) const{
						return texture == rhs.texture && sampler == rhs.sampler;
					}
				};

				TextureSamplerHandle handles;
				handles.texture = m_table->getBindlessHandle(desc.slot+0);
				handles.sampler = m_table->getBindlessHandle(desc.slot+1);
				setValueProprty(name, MaterialPropertyType::Texture, handles);
			}

		}
	}


	//! @brief  Bufferプロパティを設定
	void MaterialBlock::setBuffer(StringView name, const Ref<rhi::Buffer>& value) {

		if (!m_table) {
			LOG_ERROR("MaterialBlockの構築に失敗しています [{}]", name);
			return;
		}

		bool useBindless = rhi::RHI::Instance().getConfig().enableBindless;

		if (auto found = m_properties.find(name); found != m_properties.end()) {

			auto& desc = found->second;

			// バリデート
			if (desc.type != MaterialPropertyType::Buffer)return;
			if (useBindless) {
				if (!is_in_range(desc.offset + sizeof(rhi::BindlessHandle) - 1, m_values))return;
			} else {
				if (!is_in_range(desc.index, m_buffers))return;
			}

			m_buffers[desc.index] = value;

			m_table->setResource(desc.slot, value);

			if (useBindless) {
				rhi::BindlessHandle handle = m_table->getBindlessHandle(desc.slot);
				setValueProprty(name, MaterialPropertyType::Buffer, handle);
			}

		}
	}


	//! @brief MaterialBlockのハンドルを指定のスロットに記録する
	//! @details Bindless時のみ使用可能です。
	//!          この関数を呼び出すと、指定のスロットに対してMaterialBlockのBufferHandle記録されます。
	void MaterialBlock::record(Ref<CommandList>& commandList, s32 slot, s32 offset) {
		if (!commandList) return;
		if (!m_table) return;
		using namespace ob::rhi;

		updateParameterBuffer();

		if (m_isBindless) {

			size_t valuesIndex = m_textures.size() * 2 + m_buffers.size();

			rhi::BindlessHandle handle = m_table->getBindlessHandle(valuesIndex);

			SetRootConstantsParam param;
			param.blob = BlobView(&handle, sizeof(handle));
			param.offset = offset;
			commandList->setRootConstant(param);

		} else {

			SetDescriptorTableParam param;
			param.slot = slot;
			param.table = m_table;
			commandList->setRootDesciptorTable(&param,1);

		}

	}


	//! @brief パラメーターバッファを更新する
	void MaterialBlock::updateParameterBuffer() {
		if (m_hasChanged) {
			m_valuesBuffer->updateDirect(m_values.size(), m_values.data());
			m_hasChanged = false;
		}
	}

}