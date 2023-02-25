//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include <Framework/Graphic/Material.h>
#include <Framework/Graphic/Material/MaterialImpl.h>
#include <Framework/Graphic/Material/MaterialManager.h>
namespace ob::graphic {

    //@―---------------------------------------------------------------------------
    //! @brief  生成
    //@―---------------------------------------------------------------------------
    Ref<Material> Material::Create(const MaterialDesc& desc) {
        return new MaterialImpl(desc);
    }

    //@―---------------------------------------------------------------------------
    //! @brief  
    //@―---------------------------------------------------------------------------
    void Material::RegisterRenderPass(engine::Name name, const Ref<rhi::RenderPass>& renderPass, s32 subpass) {
        MaterialManager::Get().registerRenderPass(name, renderPass, subpass);
    }

    //@―---------------------------------------------------------------------------
    //! @brief  
    //@―---------------------------------------------------------------------------
    rhi::SubPass Material::FindRenderPass(engine::Name renderTag) {
        return MaterialManager::Get().FindRenderPass(renderTag);
    }

    //@―---------------------------------------------------------------------------
    //! @brief  グローバルFloatプロパティを設定する
    //@―---------------------------------------------------------------------------
    void Material::SetGlobalFloat(StringView name, f32 value) {
        MaterialManager::Get().setFloat(name, value);
    }
    //@―---------------------------------------------------------------------------
    //! @brief  グローバルColorプロパティを設定する
    //@―---------------------------------------------------------------------------
    void Material::SetGlobalColor(StringView name, Color value) {
        MaterialManager::Get().setColor(name, value);
    }
    //@―---------------------------------------------------------------------------
    //! @brief  グローバルMatrixプロパティを設定する
    //@―---------------------------------------------------------------------------
    void Material::SetGlobalMatrix(StringView name, const Matrix& value) {
        MaterialManager::Get().setMatrix(name, value);
    }
    //@―---------------------------------------------------------------------------
    //! @brief  グローバルTextureプロパティを設定する
    //@―---------------------------------------------------------------------------
    void Material::SetGlobalTexture(StringView name, const Ref<Texture>& value) {
        MaterialManager::Get().setTexture(name, value);
    }

}// namespace ob