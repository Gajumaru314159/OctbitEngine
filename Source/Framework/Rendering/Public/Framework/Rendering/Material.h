//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphic/Texture.h>
#include <Framework/Rendering/MaterialDesc.h>

namespace ob::graphic {
    class CommandList;
}

namespace ob::rendering {

    

    //@―---------------------------------------------------------------------------
    //! @brief  説明
    //@―---------------------------------------------------------------------------
    class Material {
    public:

        //===============================================================
        // コンストラクタ / デストラクタ
        //===============================================================

        Material() = default;

        //@―---------------------------------------------------------------------------
        //! @brief  説明
        //@―---------------------------------------------------------------------------
        Material(const MaterialDesc& desc);

        bool hasInt(StringView name)const;
        bool hasFloat(StringView name)const;
        bool hasColor(StringView name)const;
        bool hasMatrix(StringView name)const;
        bool hasTexture(StringView name)const;

        void setFloat(StringView name, f32 value);
        void setColor(StringView name, Color value);
        void setMatrix(StringView name, const Matrix& value);
        void setTexture(StringView name, const graphic::Texture& value);
        //void setFloatArray(StringView name, Span<f32> values);
        //void setColorArray(StringView name, Span<Color> values);
        //void setBuffer(StringView name, Buffer );

    public:

        void record(graphic::CommandList&);

    private:

        UPtr<class MaterialImpl> m_impl;

    };






    //===============================================================
    // インライン関数
    //===============================================================
    //! @cond



    //! @endcond
}// namespcae ob