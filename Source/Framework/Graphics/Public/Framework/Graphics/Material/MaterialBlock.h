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
        Vector<String>  textures;
        Vector<String>  buffers;
        Vector<String>  matrices;
        Vector<String>  vectors;
        Vector<String>  scalars;
        Vector<String>  integers;
    };

	//! @brief      マテリアルのパラメーターを管理するクラス
    //! 
	//! MaterialBlockはBindlessとBindfullの両方のモードに対応しています。
    //! ただし、内部的な挙動が変わります。
    //! 
    //! ### Bindfull
	//! * Integer, Scalar, Vector, Matrixのプロパティがパッキングされたバッファ (CBV)
    //! * Texture (SRV)
	//! * Sampler (Sampler)
	//! * Buffer (SRV)
    //! 
    //! ### Bindless
	//! * 「Integer, Scalar, Vector, Matrix、Textureのハンドル、Samplerのハンドル、Bufferのハンドルがパッキングされたバッファ」のハンドル (CBV)
    //! 
    //! 
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
        bool hasInteger(StringView name)const;
        bool hasScalar(StringView name)const;
        bool hasVector(StringView name)const;
        bool hasMatrix(StringView name)const;
        bool hasTexture(StringView name)const;
        bool hasBuffer(StringView name)const;
        //! @}

        //! @brief  マテリアルパラメータを設定
        //! @{
        void setInteger(StringView name, s32 value);
        void setScalar(StringView name, f32 value);
        void setVector(StringView name, Color value);
        void setMatrix(StringView name, const Matrix& value);
        void setTexture(StringView name, const Ref<Texture>& texture, const Ref<Sampler>& sampler);
        void setBuffer(StringView name, const Ref<Buffer>& value);
        //! @}

        //! @brief MaterialBlockのハンドルを指定のスロットに記録する
        //!@details Bindfull時のみ使用可能です。
        //!         この関数を呼び出すと、指定のスロットに対してDescriptorTableが設定されます。
		//!         Slotに-1が指定された場合、そのスロットには記録されません。
        //!         CBVの先頭にはパラメータのバッファが記録され、その後ろにBufferのリストが記録されます。
		//!         詳細は単体テストを参照してください。
        void record(Ref<CommandList>& commandList, s32 srvSlot, s32 uavSlot, s32 samplerSlot);

		//! @brief MaterialBlockのハンドルを指定のスロットに記録する
		//! @details Bindless時のみ使用可能です。
        //!          この関数を呼び出すと、指定のスロットに対してMaterialBlockのBufferHandle記録されます。
        void record(Ref<CommandList>& commandList, s32 slot);

    private:

        void initializeProperties(const MaterialBlockDesc& desc);
        void initializeDescriptorTables();

        template<typename T, typename TEq = std::equal_to<T>>
        void setValueProprty(StringView name, MaterialPropertyType type, const T& value);

		void updateParameterBuffer();

    private:

        union Component {
            float f;
            int i;
            unsigned int u;
        };

        MaterialPropertyMap     m_properties;
        
        bool					m_isBindless = false;
        bool					m_hasChanged = false;
        Blob            		m_values;

		Span<Component>         m_valuesDebug;

        Ref<Buffer>	            m_valuesBuffer;
        Vector<Ref<Texture>>    m_textures;
        Vector<Ref<Sampler>>    m_samplers;
        Vector<Ref<Buffer>>     m_buffers;


        // Ref<DescriptorTable>    m_tableCBV;
        Ref<DescriptorTable>    m_tableSRV;
        Ref<DescriptorTable>    m_tableUAV;
        Ref<DescriptorTable>    m_tableSampler;

    };



    inline bool MaterialBlock::hasInteger(StringView name)const { return hasProprty(name, MaterialPropertyType::Integer); }
    inline bool MaterialBlock::hasScalar(StringView name)const { return hasProprty(name, MaterialPropertyType::Scalar); }
    inline bool MaterialBlock::hasVector(StringView name)const { return hasProprty(name, MaterialPropertyType::Vector); }
    inline bool MaterialBlock::hasMatrix(StringView name)const { return hasProprty(name, MaterialPropertyType::Matrix); }
    inline bool MaterialBlock::hasTexture(StringView name)const { return hasProprty(name, MaterialPropertyType::Texture); }
    inline bool MaterialBlock::hasBuffer(StringView name)const { return hasProprty(name, MaterialPropertyType::Buffer); }

}