//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphics/Material/MaterialBlock.h>
#include <Framework/RHI/Buffer.h>
#include <Framework/RHI/Texture.h>
#include <Framework/RHI/CommandList.h>
#include <Framework/RHI/DescriptorTable.h>

namespace ob::graphics {

	static bool s_useBindless = false;

    struct BufferHandle {
        u32 index;

		bool operator==(const BufferHandle& rhs)const { return index == rhs.index; }
    };
    struct TextureHandle {
        u32 index;

        bool operator==(const TextureHandle& rhs)const { return index == rhs.index; }
    };
    struct SamplerHandle {
        u32 index;

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
        initializeDescriptorTables(desc);
    }

    //! @brief プロパティを初期化する
    //! @param desc マテリアルブロックの説明
    void MaterialBlock::initializeProperties(const MaterialBlockDesc& desc) {
        using namespace ob::rhi;

        bool useBindless = s_useBindless;

        // 定数変数のパッキング 規則 (https://learn.microsoft.com/ja-jp/windows/win32/direct3dhlsl/dx-graphics-hlsl-packing-rules)
        constexpr auto alignment = sizeof(f32)*4;

        // プロパティ名とオフセットを対応
        s32 bufferSize = 0;
        for (auto& name : desc.scalars) {
            auto [itr, added] = m_properties.try_emplace(name, MaterialValuePropertyDesc{ MaterialPropertyType::Float,bufferSize });
            if (!added) { LOG_ERROR("プロパティ[{}]はマテリアルに既に含まれています。", name); return; }
            bufferSize += sizeof(f32);
        }
        bufferSize = align_up(bufferSize, alignment);

        for (auto& name : desc.colors) {
            auto [itr, added] = m_properties.try_emplace(name, MaterialValuePropertyDesc{ MaterialPropertyType::Color,bufferSize });
            if (!added) { LOG_ERROR("プロパティ[{}]はマテリアルに既に含まれています。", name); return; }
            bufferSize += sizeof(Color);
        }
        bufferSize = align_up(bufferSize, alignment);

        for (auto& name : desc.matrices) {
            auto [itr, added] = m_properties.try_emplace(name, MaterialValuePropertyDesc{ MaterialPropertyType::Matrix,bufferSize });
            if (!added) { LOG_ERROR("プロパティ[{}]はマテリアルに既に含まれています。", name); return; }
            bufferSize += sizeof(Matrix);
        }
        bufferSize = align_up(bufferSize, alignment);

        if (useBindless) {

            // バインドレス時はパラメーターバッファ内にハンドルを格納

            for (auto [index, name] : Indexed(desc.textures)) {
                auto [itr, added] = m_properties.try_emplace(name, MaterialValuePropertyDesc{ MaterialPropertyType::Texture,bufferSize });
                if (!added) { LOG_ERROR("プロパティ[{}]はマテリアルに既に含まれています。", name); return; }
                bufferSize += sizeof(TextureAndSamplerHandle);
            }
            m_textures.resize(desc.textures.size());
            m_samplers.resize(desc.textures.size());

            for (auto [index, name] : Indexed(desc.buffers)) {
                auto [itr, added] = m_properties.try_emplace(name, MaterialValuePropertyDesc{ MaterialPropertyType::Buffer,bufferSize });
                if (!added) { LOG_ERROR("プロパティ[{}]はマテリアルに既に含まれています。", name); return; }
                bufferSize += sizeof(BufferHandle);
            }
            m_buffers.resize(desc.buffers.size());

        } else {

            // バインドフル時は専用のDescriptorTableにリソースを格納

            for (auto [index, name] : Indexed(desc.textures)) {
                auto [itr, added] = m_properties.try_emplace(name, MaterialValuePropertyDesc{ MaterialPropertyType::Texture,(s32)index });
                if (!added) { LOG_ERROR("プロパティ[{}]はマテリアルに既に含まれています。", name); return; }
            }
            m_textures.resize(desc.textures.size());
            m_samplers.resize(desc.textures.size());

            for (auto [index, name] : Indexed(desc.buffers)) {
                auto [itr, added] = m_properties.try_emplace(name, MaterialValuePropertyDesc{ MaterialPropertyType::Buffer,(s32)index });
                if (!added) { LOG_ERROR("プロパティ[{}]はマテリアルに既に含まれています。", name); return; }
            }
            m_buffers.resize(desc.buffers.size());

        }

        // バッファ生成
        bufferSize = std::max(bufferSize, 4);

        if (0 < bufferSize) {
            auto bufferDesc = rhi::BufferDesc::Constant(bufferSize, rhi::BindFlag::AllShaderResource);
            bufferDesc.name = Format("MaterialParameter ({})", desc.name);
            m_parameterBuffer = rhi::Buffer::Create(bufferDesc);
            OB_ASSERT_EXPR(m_parameterBuffer);
            m_parameterBufferBlob.resize(bufferSize);
            memset(m_parameterBufferBlob.data(), 0, m_parameterBufferBlob.size());
        }

    }

    //! @brief デスクリプタテーブルを初期化する
    //! @param desc マテリアルブロックの説明
    void MaterialBlock::initializeDescriptorTables(const MaterialBlockDesc& desc) {
        using namespace ob::rhi;

        bool useBindless = s_useBindless;

        if (useBindless) {
            initializeBindlessDescriptorTables(desc);
        } else {
            initializeBindfullDescriptorTables(desc);
        }

    }

    //! @brief バインドレスデスクリプタテーブルを初期化する
    //! @param desc マテリアルブロックの説明
    void MaterialBlock::initializeBindlessDescriptorTables([[maybe_unused]] const MaterialBlockDesc& desc) {
        using namespace ob::rhi;

        m_tables[0] = rhi::DescriptorTable::Create(DescriptorHeapType::CBV_SRV_UAV, 1);
        m_tables[0]->setResource(0, m_parameterBuffer);
    }

    //! @brief バインドフルデスクリプタテーブルを初期化する
    //! @param desc マテリアルブロックの説明
    void MaterialBlock::initializeBindfullDescriptorTables(const MaterialBlockDesc& desc) {
        using namespace ob::rhi;

        size_t resourceNum = desc.textures.size() + desc.buffers.size() + 1;
        size_t samplerNum = desc.textures.size();

        m_tables[0] = rhi::DescriptorTable::Create(DescriptorHeapType::CBV_SRV_UAV, resourceNum);
        m_tables[1] = rhi::DescriptorTable::Create(DescriptorHeapType::Sampler, samplerNum);

        // テーブル初期化
        s32 index = 0;
        {
            m_tables[0]->setResource(index, m_parameterBuffer);
        }
        for (s32 i = 0; i < desc.textures.size(); ++i) {
            m_tables[0]->setResource(index, Texture::White());
            // TODO サンプラ設定
            //m_tables[1]->setResource(i, Texture::White());
        }
        for (s32 i = 0; i < desc.buffers.size(); ++i) {
            // TODO デフォルトバッファ指定
            // m_tables[0]->setResource(index, Buffer::Empty());
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
            if (!is_in_range(desc.offset, m_parameterBufferBlob))return;

            auto& dest = *GetOffsetPtr<T>(m_parameterBufferBlob.data(), desc.offset);

            if (TEq()(value, dest))return;

            dest = value;
        }
    }

    //! @brief  Floatプロパティを設定
    void MaterialBlock::setFloat(StringView name, f32 value) {
        setValueProprty(name, MaterialPropertyType::Float, value);
    }

    //! @brief  Colorプロパティを設定
    void MaterialBlock::setColor(StringView name, Color value) {
        setValueProprty(name, MaterialPropertyType::Color, value);
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

        bool useBindless = s_useBindless;

        if (auto found = m_properties.find(name); found != m_properties.end()) {

            auto& desc = found->second;
            if (desc.type != MaterialPropertyType::Texture)return;
            if (!is_in_range(desc.offset, m_textures))return;

            m_textures[desc.offset] = texture;
            m_samplers[desc.offset] = sampler;

            if (useBindless) {
				TextureAndSamplerHandle handle{ };
                setValueProprty(name, MaterialPropertyType::Texture, handle);
            } else {
                m_tables[0]->setResource(desc.offset, texture);
                m_tables[1]->setResource(desc.offset, sampler);
            }

        }
    }

    //! @brief  Bufferプロパティを設定
    void MaterialBlock::setBuffer(StringView name, const Ref<rhi::Buffer>& value) {

        bool useBindless = s_useBindless;

        if (auto found = m_properties.find(name); found != m_properties.end()) {

            auto& desc = found->second;
            if (desc.type != MaterialPropertyType::Buffer)return;
            if (!is_in_range(desc.offset, m_buffers))return;

            m_buffers[desc.offset] = value;

            if (useBindless) {
                BufferHandle handle{ };
                setValueProprty(name, MaterialPropertyType::Buffer, handle);
            }
            else {
                m_tables[0]->setResource(desc.offset, value);
            }

        }
    }

    //! @brief コマンドリストにデスクリプタテーブルを設定する
    //! @param commandList コマンドリスト
    //! @param resourceSlot リソース用のDescriptorTableを設定するスロット
    //! @param samplerSlot サンプラー用のDescriptorTableを設定するスロット
    void MaterialBlock::record(Ref<CommandList>& commandList, s32 resourceSlot, s32 samplerSlot) {
        if (!commandList) return;
        using namespace ob::rhi;

        SetDescriptorTableParam params[2];
        params[0].table = m_tables[0];
        params[0].slot = resourceSlot;
        params[1].table = m_tables[1];
        params[1].slot = samplerSlot;
        commandList->setRootDesciptorTable(params, std::size(params));
    }

}