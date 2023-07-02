//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/Forward.h>
#include <Framework/RHI/Types/SubPass.h>
#include <Framework/Graphics/Forward.h>
#include <Framework/Graphics/MaterialDesc.h>

namespace ob::graphics {   

    //@―---------------------------------------------------------------------------
    //! @brief  マテリアル
    //@―---------------------------------------------------------------------------
    class Material:public RefObject {
    public:
        using Texture = ob::rhi::Texture;
    public:

        //@―---------------------------------------------------------------------------
        //! @brief  生成
        //@―---------------------------------------------------------------------------
        static Ref<Material> Create(const MaterialDesc& desc);

        //@―---------------------------------------------------------------------------
        //! @brief  マテリアルが使用するRenderPassを登録
        //@―---------------------------------------------------------------------------
        static Ref<rhi::RenderPass> AddRenderPass(const rhi::RenderPassDesc& desc);

        //@―---------------------------------------------------------------------------
        //! @brief  レンダーパスを検索
        //@―---------------------------------------------------------------------------
        static rhi::SubPass FindSubpass(Name renderTag);

    public:

        //@―---------------------------------------------------------------------------
        //! @brief  マテリアルパラメータが存在するか
        //@―---------------------------------------------------------------------------
        //! @{
        virtual bool hasInt(StringView name)const = 0;
        virtual bool hasFloat(StringView name)const = 0;
        virtual bool hasColor(StringView name)const = 0;
        virtual bool hasMatrix(StringView name)const = 0;
        virtual bool hasTexture(StringView name)const = 0;
        //! @}
        
        //@―---------------------------------------------------------------------------
        //! @brief  マテリアルパラメータを設定
        //@―---------------------------------------------------------------------------
        //! @{
        virtual void setFloat(StringView name, f32 value) = 0;
        virtual void setColor(StringView name, Color value) = 0;
        virtual void setMatrix(StringView name, const Matrix& value) = 0;
        virtual void setTexture(StringView name, const Ref<Texture>& value) = 0;
        //! @}
        
        //virtual void setFloatArray(StringView name, Span<f32> values);
        //virtual void setColorArray(StringView name, Span<Color> values);
        //virtual void setBuffer(StringView name, Buffer );

        //@―---------------------------------------------------------------------------
        //! @brief  GPUリソースの事前生成
        //! @details GPUリソースを事前生成しておくことで描画時のスパイクを回避することができます。
        //@―---------------------------------------------------------------------------
        virtual bool reserve(const Ref<Mesh>& mesh) = 0;

    public:

        //@―---------------------------------------------------------------------------
        //! @brief  グローバルマテリアルパラメータを設定
        //@―---------------------------------------------------------------------------
        //! @{
        static void SetGlobalFloat(StringView name, f32 value);
        static void SetGlobalColor(StringView name, Color value);
        static void SetGlobalMatrix(StringView name, const Matrix& value);
        static void SetGlobalTexture(StringView name, const Ref<Texture>& value);
        //! @}

    };

}