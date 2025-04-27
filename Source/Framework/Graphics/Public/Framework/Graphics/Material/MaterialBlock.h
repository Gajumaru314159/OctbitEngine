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
		Ref<rhi::DescriptorLayout> layout;
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
    //! * Texture (SRV)
	//! * Sampler (Sampler)
	//! * Buffer (SRV)
	//! * Integer, Scalar, Vector, Matrixのプロパティがパッキングされたバッファ (CBV)
    //! 
    //! 上記を含むDescriptorTableを指定したスロットに対してバインドする。
    //! 
    //! ### Bindless
	//! * Integer, Scalar, Vector, Matrix、Textureのハンドル、Samplerのハンドル、Bufferのハンドルがパッキングされたバッファ
    //! 
    //! 上記のバッファのBindlessHandleを指定したオフセットのRootConstantsに書き込む。
    //! 
    //! @ref MaterialBlock
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
        bool hasProperty(StringView name, MaterialPropertyType type)const;
        bool hasInteger(StringView name)const;  //!< @copybrief hasProperty()
        bool hasScalar(StringView name)const;   //!< @copybrief hasProperty()
        bool hasVector(StringView name)const;   //!< @copybrief hasProperty()
        bool hasMatrix(StringView name)const;   //!< @copybrief hasProperty()
        bool hasTexture(StringView name)const;  //!< @copybrief hasProperty()
        bool hasBuffer(StringView name)const;   //!< @copybrief hasProperty()

        //! @brief  マテリアルパラメータを設定
        void setInteger(StringView name, s32 value);
        void setScalar(StringView name, f32 value);                 //!< @copybrief setInteger()
        void setVector(StringView name, Color value);               //!< @copybrief setInteger()
        void setMatrix(StringView name, const Matrix& value);       //!< @copybrief setInteger()
        void setTexture(StringView name, const Ref<Texture>& texture, const Ref<Sampler>& sampler); //!< @copybrief setInteger()
        void setBuffer(StringView name, const Ref<Buffer>& value);  //!< @copybrief setInteger()

		//! @brief MaterialBlockのハンドルを指定のスロットに記録する
		//! @details Bindfullの場合はslotにRootSignatureのスロットを指定する  
        //!          Bindfullの場合はoffsetにBindlessHandleを書き込むオフセットを指定する (通常sizeof(BindlessHandle)の倍数)  
        //!          BindfullとBindlessの両方に対応しやすいようにモードによらずこの関数を使用します。
        void record(Ref<CommandList>& commandList, s32 slot, s32 offset = 0);

    private:

        void initializeProperties(const MaterialBlockDesc& desc);
        void initializeDescriptorTables(const MaterialBlockDesc& desc);

        template<typename T, typename TEq = std::equal_to<T>>
        void setValueProprty(StringView name, MaterialPropertyType type, const T& value);

		void updateParameterBuffer();

    private:

        MaterialPropertyMap     m_properties;
        
        bool					m_isBindless = false;
        bool					m_hasChanged = false;
        Blob            		m_values;

        Ref<Buffer>	            m_valuesBuffer;
        Vector<Ref<Texture>>    m_textures;
        Vector<Ref<Sampler>>    m_samplers;
        Vector<Ref<Buffer>>     m_buffers;

        Ref<DescriptorTable>    m_table;

#define OB_MATERIAL_BLOCK_DEBUG_ENABLED OB_DEBUG
#if OB_MATERIAL_BLOCK_DEBUG_ENABLED
        union Component {
            f32 f;
            s32 i;
            u32 u;
        };
        Span<Component>         m_valuesDebug;
#endif

    };



    inline bool MaterialBlock::hasInteger(StringView name)const { return hasProperty(name, MaterialPropertyType::Integer); }
    inline bool MaterialBlock::hasScalar(StringView name)const { return hasProperty(name, MaterialPropertyType::Scalar); }
    inline bool MaterialBlock::hasVector(StringView name)const { return hasProperty(name, MaterialPropertyType::Vector); }
    inline bool MaterialBlock::hasMatrix(StringView name)const { return hasProperty(name, MaterialPropertyType::Matrix); }
    inline bool MaterialBlock::hasTexture(StringView name)const { return hasProperty(name, MaterialPropertyType::Texture); }
    inline bool MaterialBlock::hasBuffer(StringView name)const { return hasProperty(name, MaterialPropertyType::Buffer); }

}