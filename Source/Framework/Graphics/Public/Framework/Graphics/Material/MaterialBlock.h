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
	//! @details マテリアルのプロパティ゙定義するための構造体です。
	//!          layout を指定しない場合はBindlessモードとして扱われます。
    struct MaterialBlockDesc {
        String          name;
        Vector<String>  textures;
        Vector<String>  buffers;
        Vector<String>  matrices;
        Vector<String>  vectors;
        Vector<String>  scalars;
        Vector<String>  integers;
		Ref<rhi::DescriptorLayout> layout;
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
        using DescriptorLayout = ob::rhi::DescriptorLayout;
        using DescriptorTable = ob::rhi::DescriptorTable;
        using Sampler = ob::rhi::Sampler;
        using Texture = ob::rhi::Texture;
        using Buffer = ob::rhi::Buffer;
    public:
        //! @brief MaterialBlockDescに対応するDescriptorLayoutを生成するユーティリティ関数
        static Ref<rhi::DescriptorLayout> CreateLayout(const MaterialBlockDesc& desc);
    public:

        MaterialBlock() = default;
        MaterialBlock(const MaterialBlockDesc& desc);

        //! @brief  マテリアルパラメータが存在するか
        bool hasProperty(StringView name, MaterialPropertyType type)const;
        bool hasTexture(StringView name)const;  //!< @copybrief hasProperty()
        bool hasBuffer(StringView name)const;   //!< @copybrief hasProperty()
        bool hasMatrix(StringView name)const;   //!< @copybrief hasProperty()
        bool hasVector(StringView name)const;   //!< @copybrief hasProperty()
        bool hasScalar(StringView name)const;   //!< @copybrief hasProperty()
        bool hasInteger(StringView name)const;  //!< @copybrief hasProperty()

        //! @brief  マテリアルパラメータを設定
        void setTexture(StringView name, const Ref<Texture>& texture, const Ref<Sampler>& sampler);
        void setBuffer(StringView name, const Ref<Buffer>& value);      //!< @copybrief setTexture()
        void setMatrix(StringView name, const Matrix& value);           //!< @copybrief setTexture()
        void setVector(StringView name, Color value);                   //!< @copybrief setTexture()
        void setScalar(StringView name, f32 value);                     //!< @copybrief setTexture()
        void setInteger(StringView name, s32 value);                    //!< @copybrief setTexture()

		//! @brief MaterialBlockのハンドルを指定のスロットに記録する
		//! @details Bindfullの場合はslotにRootSignatureのスロットを指定する  
        //!          Bindfullの場合はRootConstantsのoffsetにBindlessHandleを書き込む  
        //!          BindfullとBindlessの両方に対応しやすいようにモードによらずこの関数を使用します。
        void record(Ref<CommandList>& commandList, s32 slot);

		//! @brief  レイアウトを取得
        const Ref<DescriptorLayout>& getLayout()const;

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

        Ref<DescriptorLayout>   m_layout;
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



    inline bool MaterialBlock::hasTexture(StringView name)const { return hasProperty(name, MaterialPropertyType::Texture); }
    inline bool MaterialBlock::hasBuffer(StringView name)const { return hasProperty(name, MaterialPropertyType::Buffer); }
    inline bool MaterialBlock::hasMatrix(StringView name)const { return hasProperty(name, MaterialPropertyType::Matrix); }
    inline bool MaterialBlock::hasVector(StringView name)const { return hasProperty(name, MaterialPropertyType::Vector); }
    inline bool MaterialBlock::hasScalar(StringView name)const { return hasProperty(name, MaterialPropertyType::Scalar); }
    inline bool MaterialBlock::hasInteger(StringView name)const { return hasProperty(name, MaterialPropertyType::Integer); }

}