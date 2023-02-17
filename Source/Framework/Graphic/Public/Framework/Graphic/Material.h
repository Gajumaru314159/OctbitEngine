//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/Texture.h>
#include <Framework/Graphic/MaterialDesc.h>
#include <Framework/Engine/Name.h>

namespace ob::rhi {
    class CommandList;
}

namespace ob::graphic {

    

    //@―---------------------------------------------------------------------------
    //! @brief  説明
    //@―---------------------------------------------------------------------------
    class Material {
    public:
        using Texture = rhi::Texture;
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
        void setTexture(StringView name, const Ref<Texture>& value);
        //void setFloatArray(StringView name, Span<f32> values);
        //void setColorArray(StringView name, Span<Color> values);
        //void setBuffer(StringView name, Buffer );

    public:

        static void SetGlobalFloat(StringView name, f32 value);
        static void SetGlobalColor(StringView name, Color value);
        static void SetGlobalMatrix(StringView name, const Matrix& value);
        static void SetGlobalTexture(StringView name, const Ref<Texture>& value);

    public:

        //@―---------------------------------------------------------------------------
        //! @brief  描画コマンドを記録
        //@―---------------------------------------------------------------------------
        void record(Ref<rhi::CommandList>&,engine::Name pass)const;

    private:

        UPtr<class MaterialImpl> m_impl;

    };






    //===============================================================
    // インライン関数
    //===============================================================
    //! @cond



    //! @endcond
}// namespcae ob