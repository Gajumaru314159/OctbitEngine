//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include <Framework/Graphics/Material.h>
#include <Framework/Graphics/Material/MaterialImpl.h>
#include <Framework/Graphics/Material/MaterialManager.h>
namespace ob::graphics {

    //@―---------------------------------------------------------------------------
    //! @brief  生成
    //@―---------------------------------------------------------------------------
    Ref<Material> Material::Create(const MaterialDesc& desc) {
        return new MaterialImpl(desc);
    }

    //@―---------------------------------------------------------------------------
    //! @brief  
    //@―---------------------------------------------------------------------------
    void Material::RegisterRenderPass(Name name, const Ref<rhi::RenderPass>& renderPass, s32 subpass) {
        if (auto manager = MaterialManager::Get()) {
            manager->registerRenderPass(name, renderPass, subpass);
        }
    }

    //@―---------------------------------------------------------------------------
    //! @brief  
    //@―---------------------------------------------------------------------------
    rhi::SubPass Material::FindRenderPass(Name renderTag) {
        if (auto manager = MaterialManager::Get()) {
            return manager->FindRenderPass(renderTag);
        }
        return {};
    }

    //@―---------------------------------------------------------------------------
    //! @brief  グローバルFloatプロパティを設定する
    //@―---------------------------------------------------------------------------
    void Material::SetGlobalFloat(StringView name, f32 value) {
        if (auto manager = MaterialManager::Get()) {
            manager->setFloat(name, value);
        }
    }
    //@―---------------------------------------------------------------------------
    //! @brief  グローバルColorプロパティを設定する
    //@―---------------------------------------------------------------------------
    void Material::SetGlobalColor(StringView name, Color value) {
        if (auto manager = MaterialManager::Get()) {
            manager->setColor(name, value);
        }
    }
    //@―---------------------------------------------------------------------------
    //! @brief  グローバルMatrixプロパティを設定する
    //@―---------------------------------------------------------------------------
    void Material::SetGlobalMatrix(StringView name, const Matrix& value) {
        if (auto manager = MaterialManager::Get()) {
            manager->setMatrix(name, value);
        }
    }
    //@―---------------------------------------------------------------------------
    //! @brief  グローバルTextureプロパティを設定する
    //@―---------------------------------------------------------------------------
    void Material::SetGlobalTexture(StringView name, const Ref<Texture>& value) {
        if (auto manager = MaterialManager::Get()) {
            manager->setTexture(name, value);
        }
    }

}