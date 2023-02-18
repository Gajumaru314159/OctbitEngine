//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/Texture.h>
#include <Framework/Graphic/MaterialDesc.h>
#include <Framework/Engine/Name.h>
#include <Framework/RHI/Types/SubPass.h>

namespace ob::graphic {   

    //@―---------------------------------------------------------------------------
    //! @brief  マテリアル
    //@―---------------------------------------------------------------------------
    class Material:public RefObject {
    public:
        using Texture = rhi::Texture;
    public:

        //@―---------------------------------------------------------------------------
        //! @brief  マテリアルを生成
        //@―---------------------------------------------------------------------------
        static Ref<Material> Create(const MaterialDesc& desc);

        static void RegisterRenderPass(engine::Name name, const Ref<rhi::RenderPass>&, s32 subpass);
        static rhi::SubPass FindRenderPass(engine::Name renderTag);


    public:

        virtual ~Material() = default;

        virtual bool hasInt(StringView name)const = 0;
        virtual bool hasFloat(StringView name)const = 0;
        virtual bool hasColor(StringView name)const = 0;
        virtual bool hasMatrix(StringView name)const = 0;
        virtual bool hasTexture(StringView name)const = 0;

        virtual void setFloat(StringView name, f32 value) = 0;
        virtual void setColor(StringView name, Color value) = 0;
        virtual void setMatrix(StringView name, const Matrix& value) = 0;
        virtual void setTexture(StringView name, const Ref<Texture>& value) = 0;
        //virtual void setFloatArray(StringView name, Span<f32> values);
        //virtual void setColorArray(StringView name, Span<Color> values);
        //virtual void setBuffer(StringView name, Buffer );

    public:

        static void SetGlobalFloat(StringView name, f32 value);
        static void SetGlobalColor(StringView name, Color value);
        static void SetGlobalMatrix(StringView name, const Matrix& value);
        static void SetGlobalTexture(StringView name, const Ref<Texture>& value);

    };






    //===============================================================
    // インライン関数
    //===============================================================
    //! @cond



    //! @endcond
}// namespcae ob