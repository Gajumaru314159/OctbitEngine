//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include <Framework/Graphic/Material.h>
#include <Framework/Graphic/Material/MaterialImpl.h>

namespace ob::graphic {

    //@―---------------------------------------------------------------------------
    //! @brief  生成
    //@―---------------------------------------------------------------------------
    Ref<Material> Material::Create(const MaterialDesc& desc) {
        return new MaterialImpl(desc);
    }


    void Material::RegisterRenderPass(engine::Name name, const Ref<rhi::RenderPass>&, s32 subpass) {

    }

    Pair<const Ref<rhi::RenderPass>, s32> Material::FindRenderPass(engine::Name renderTag) {
        // TODO

        return { nullptr,0 };
    }

    //@―---------------------------------------------------------------------------
    //! @brief  グローバルFloatプロパティを設定する
    //@―---------------------------------------------------------------------------
    void Material::SetGlobalFloat(StringView name, f32 value) {
        OB_NOTIMPLEMENTED();
    }
    //@―---------------------------------------------------------------------------
    //! @brief  グローバルColorプロパティを設定する
    //@―---------------------------------------------------------------------------
    void Material::SetGlobalColor(StringView name, Color value) {
        OB_NOTIMPLEMENTED();
    }
    //@―---------------------------------------------------------------------------
    //! @brief  グローバルMatrixプロパティを設定する
    //@―---------------------------------------------------------------------------
    void Material::SetGlobalMatrix(StringView name, const Matrix& value) {
        OB_NOTIMPLEMENTED();
    }
    //@―---------------------------------------------------------------------------
    //! @brief  グローバルTextureプロパティを設定する
    //@―---------------------------------------------------------------------------
    void Material::SetGlobalTexture(StringView name, const Ref<Texture>& value) {
        OB_NOTIMPLEMENTED();
    }

}// namespace ob