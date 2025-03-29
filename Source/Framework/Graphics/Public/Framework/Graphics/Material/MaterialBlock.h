//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/Forward.h>
#include <Framework/Graphics/Material/MaterialTypes.h>

namespace ob::graphics {

    //! @brief  マテリアル定義
    struct MaterialBlockDesc {
        String          name;
        Vector<String>  scalars;
        Vector<String>  colors;
        Vector<String>  matrices;
        Vector<String>  textures;
        Vector<String>  buffers;
    };

	//! @brief      マテリアルのパラメーターを管理するクラス
    class MaterialBlock {
    public:
        using CommandList = ob::rhi::CommandList;
        using DescriptorTable = ob::rhi::DescriptorTable;
        using Sampler = ob::rhi::Sampler;
        using Texture = ob::rhi::Texture;
        using Buffer = ob::rhi::Buffer;
    public:

        MaterialBlock(const MaterialBlockDesc& desc);

        //! @brief  マテリアルパラメータが存在するか
        //! @{
        bool hasProprty(StringView name, MaterialPropertyType type)const;
        bool hasInt(StringView name)const;
        bool hasFloat(StringView name)const;
        bool hasColor(StringView name)const;
        bool hasMatrix(StringView name)const;
        bool hasTexture(StringView name)const;
        bool hasBuffer(StringView name)const;
        //! @}

        //! @brief  マテリアルパラメータを設定
        //! @{
        void setFloat(StringView name, f32 value);
        void setColor(StringView name, Color value);
        void setMatrix(StringView name, const Matrix& value);
        void setTexture(StringView name, const Ref<Texture>& texture, const Ref<Sampler>& sampler);
        void setBuffer(StringView name, const Ref<Buffer>& value);
        //! @}

        void record(Ref<CommandList>& commandList, s32 resourceSlot, s32 samplerSlot);

    private:

        void initializeProperties(const MaterialBlockDesc& desc);
        void initializeDescriptorTables(const MaterialBlockDesc& desc);
        void initializeBindlessDescriptorTables(const MaterialBlockDesc& desc);
        void initializeBindfullDescriptorTables(const MaterialBlockDesc& desc);

        template<typename T, typename TEq = std::equal_to<T>>
        void setValueProprty(StringView name, MaterialPropertyType type, const T& value);

    private:

        MaterialPropertyMap     m_properties;

        Blob				    m_parameterBufferBlob;

        Ref<rhi::Buffer>	    m_parameterBuffer;
        Vector<Ref<Texture>>    m_textures;
        Vector<Ref<Sampler>>    m_samplers;
        Vector<Ref<Buffer>>     m_buffers;

        Ref<DescriptorTable>    m_tables[2];

    };



    inline bool MaterialBlock::hasInt(StringView name)const { return hasProprty(name, MaterialPropertyType::Int); }
    inline bool MaterialBlock::hasFloat(StringView name)const { return hasProprty(name, MaterialPropertyType::Float); }
    inline bool MaterialBlock::hasColor(StringView name)const { return hasProprty(name, MaterialPropertyType::Color); }
    inline bool MaterialBlock::hasMatrix(StringView name)const { return hasProprty(name, MaterialPropertyType::Matrix); }
    inline bool MaterialBlock::hasTexture(StringView name)const { return hasProprty(name, MaterialPropertyType::Texture); }
    inline bool MaterialBlock::hasBuffer(StringView name)const { return hasProprty(name, MaterialPropertyType::Buffer); }

}