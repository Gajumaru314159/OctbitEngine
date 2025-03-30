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

    struct alignas(16) BufferHandle {
        u32 index;

		bool operator==(const BufferHandle& rhs)const { return index == rhs.index; }
    };
    struct alignas(16) TextureHandle {
        u32 index;

        bool operator==(const TextureHandle& rhs)const { return index == rhs.index; }
    };
    struct alignas(16) SamplerHandle {
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

        bool useBindless = RHI::Instance().getConfig().enableBindless;

        // NOTE ここで生成しているマップはMaterialBlockDescが同じであればシステム内で共有可能
		//      キャッシュ対応することによってメモリ消費量の削減が見込める

        // 定数変数のパッキング 規則 (https://learn.microsoft.com/ja-jp/windows/win32/direct3dhlsl/dx-graphics-hlsl-packing-rules)
        constexpr auto alignment = sizeof(f32)*4;

        // プロパティ名とオフセットを対応
        s32 bufferSize = 0;

        for (auto& name : desc.integers) {
            auto [itr, added] = m_properties.try_emplace(name, MaterialValuePropertyDesc{ MaterialPropertyType::Integer,bufferSize });
            if (!added) { LOG_ERROR("プロパティ[{}]はマテリアルに既に含まれています。", name); return; }
            bufferSize += sizeof(s32);
        }
        bufferSize = align_up(bufferSize, alignment);

        for (auto& name : desc.scalars) {
            auto [itr, added] = m_properties.try_emplace(name, MaterialValuePropertyDesc{ MaterialPropertyType::Scalar,bufferSize });
            if (!added) { LOG_ERROR("プロパティ[{}]はマテリアルに既に含まれています。", name); return; }
            bufferSize += sizeof(f32);
        }
        bufferSize = align_up(bufferSize, alignment);

        for (auto& name : desc.vectors) {
            auto [itr, added] = m_properties.try_emplace(name, MaterialValuePropertyDesc{ MaterialPropertyType::Vector,bufferSize });
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
                auto [itr, added] = m_properties.try_emplace(name, MaterialValuePropertyDesc{ MaterialPropertyType::Texture,bufferSize,(s32)index });
                if (!added) { LOG_ERROR("プロパティ[{}]はマテリアルに既に含まれています。", name); return; }
                bufferSize += sizeof(TextureAndSamplerHandle);
            }
            m_textures.resize(desc.textures.size());
            m_samplers.resize(desc.textures.size());

            for (auto [index, name] : Indexed(desc.buffers)) {
                auto [itr, added] = m_properties.try_emplace(name, MaterialValuePropertyDesc{ MaterialPropertyType::Buffer,bufferSize,(s32)index });
                if (!added) { LOG_ERROR("プロパティ[{}]はマテリアルに既に含まれています。", name); return; }
                bufferSize += sizeof(BufferHandle);
            }
            m_buffers.resize(desc.buffers.size());

        } else {

            // バインドフル時は専用のDescriptorTableにリソースを格納
            for (auto [index, name] : Indexed(desc.textures)) {
                auto [itr, added] = m_properties.try_emplace(name, MaterialValuePropertyDesc{ MaterialPropertyType::Texture,(s32)index,(s32)index });
                if (!added) { LOG_ERROR("プロパティ[{}]はマテリアルに既に含まれています。", name); return; }
            }
            m_textures.resize(desc.textures.size());
            m_samplers.resize(desc.textures.size());

            for (auto [index, name] : Indexed(desc.buffers)) {
                auto [itr, added] = m_properties.try_emplace(name, MaterialValuePropertyDesc{ MaterialPropertyType::Buffer,(s32)index,(s32)index });
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
            OB_ASSERT_EXPR(bufferSize % sizeof(Component) == 0);

            m_parameterBufferBlob.resize(bufferSize / sizeof(Component));
            memset(m_parameterBufferBlob.data(), 0, bufferSize);
        }

    }

    //! @brief デスクリプタテーブルを初期化する
    //! @param desc マテリアルブロックの説明
    void MaterialBlock::initializeDescriptorTables(const MaterialBlockDesc& desc) {
        using namespace ob::rhi;

        bool useBindless = RHI::Instance().getConfig().enableBindless;

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

        m_tableCBV = rhi::DescriptorTable::Create(DescriptorHeapType::CBV_SRV_UAV, 1);
        m_tableCBV->setResource(0, m_parameterBuffer);


        size_t cbvNum = 1 + desc.buffers.size();
        size_t srvNum = desc.textures.size();
        size_t uavNum = 0;
        size_t samplerNum = desc.textures.size();

        //m_tableCBV = rhi::DescriptorTable::Create(DescriptorHeapType::CBV_SRV_UAV, cbvNum);
        m_tableSRV = rhi::DescriptorTable::Create(DescriptorHeapType::CBV_SRV_UAV, srvNum);
        m_tableUAV = rhi::DescriptorTable::Create(DescriptorHeapType::CBV_SRV_UAV, uavNum);
        m_tableSampler = rhi::DescriptorTable::Create(DescriptorHeapType::Sampler, samplerNum);

        // テーブル初期化
        {
            //m_tableCBV->setResource(0, m_parameterBuffer);
        }
        for (s32 i = 0; i < desc.buffers.size(); ++i) {
            // TODO デフォルトバッファ指定
            //m_tableCBV->setResource(i + 1, Buffer::Empty());
        }
        for (s32 i = 0; i < desc.textures.size(); ++i) {
            m_tableSRV->setResource(i, Texture::White());
            m_tableSampler->setResource(i, Sampler::Default());
        }

    }

    //! @brief バインドフルデスクリプタテーブルを初期化する
    //! @param desc マテリアルブロックの説明
    void MaterialBlock::initializeBindfullDescriptorTables(const MaterialBlockDesc& desc) {
        using namespace ob::rhi;

        size_t cbvNum = 1 + desc.buffers.size();
        size_t srvNum = desc.textures.size();
        size_t uavNum = 0;
        size_t samplerNum = desc.textures.size();

        m_tableCBV = rhi::DescriptorTable::Create(DescriptorHeapType::CBV_SRV_UAV, cbvNum);
        m_tableSRV = rhi::DescriptorTable::Create(DescriptorHeapType::CBV_SRV_UAV, srvNum);
        m_tableUAV = rhi::DescriptorTable::Create(DescriptorHeapType::CBV_SRV_UAV, uavNum);
        m_tableSampler = rhi::DescriptorTable::Create(DescriptorHeapType::Sampler, samplerNum);

        // テーブル初期化
        {
            m_tableCBV->setResource(0, m_parameterBuffer);
        }
        for (s32 i = 0; i < desc.buffers.size(); ++i) {
            // TODO デフォルトバッファ指定
            //m_tableCBV->setResource(i + 1, Buffer::Empty());
        }
        for (s32 i = 0; i < desc.textures.size(); ++i) {
            m_tableSRV->setResource(i, Texture::White());
            m_tableSampler->setResource(i, Sampler::Default());
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
            if (!is_in_range(desc.offset / sizeof(Component), m_parameterBufferBlob))return;

            auto& dest = *GetOffsetPtr<T>(m_parameterBufferBlob.data(), desc.offset);

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
            if (desc.type != MaterialPropertyType::Texture)return;

            if (useBindless) {
                if (!is_in_range(desc.offset / sizeof(Component), m_parameterBufferBlob))return;
            } else {
                if (!is_in_range(desc.offset, m_textures))return;

                // TODO Bindless時もリロード用に保持する
                m_textures[desc.index] = texture;
                m_samplers[desc.index] = sampler;
            }


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
            if (desc.type != MaterialPropertyType::Buffer)return;
            if (useBindless) {
                if (!is_in_range(desc.offset / sizeof(Component), m_parameterBufferBlob))return;
            } else {
                if (!is_in_range(desc.offset, m_buffers))return;
                m_buffers[desc.index] = value;
            }

            m_tableCBV->setResource(desc.index, value);

            if (useBindless) {
                BufferHandle handle;
				handle.index = m_tableCBV->getBindlessIndex(desc.index);
                setValueProprty(name, MaterialPropertyType::Buffer, handle);
            }

        }
    }

    //! @brief コマンドリストにデスクリプタテーブルを設定する
    //! @param commandList コマンドリスト
    //! @param resourceSlot リソース用のDescriptorTableを設定するスロット
    //! @param samplerSlot サンプラー用のDescriptorTableを設定するスロット
    void MaterialBlock::record(Ref<CommandList>& commandList, s32 cbvSlot, s32 srvSlot,s32 uavSlot,s32 samplerSlot) {
        if (!commandList) return;
        using namespace ob::rhi;
        
        bool useBindless = RHI::Instance().getConfig().enableBindless;

        if (m_hasChanged) {
            m_parameterBuffer->updateDirect(m_parameterBufferBlob.size() * sizeof(Component), m_parameterBufferBlob.data());
			m_hasChanged = false;
        }

        FixedVector < SetDescriptorTableParam, 4> params;
        if (0 <= cbvSlot) {
            auto& param = params.emplace_back();
			param.slot = cbvSlot;
			param.table = m_tableCBV;
        }
        if (!useBindless) {

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
        }

        commandList->setRootDesciptorTable(params.data(), params.size());
    }

}