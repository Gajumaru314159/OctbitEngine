﻿//***********************************************************
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
    //! @brief  マテリアルが使用するRenderPassを登録
    //@―---------------------------------------------------------------------------
    Ref<rhi::RenderPass> Material::AddRenderPass(const rhi::RenderPassDesc& desc) {
        if (auto manager = MaterialManager::Get()) {
            return manager->addRenderPass(desc);
        }
        return nullptr;
    }

    //@―---------------------------------------------------------------------------
    //! @brief  レンダーパスを検索
    //@―---------------------------------------------------------------------------
    rhi::SubPass Material::FindSubpass(Name renderTag) {
        if (auto manager = MaterialManager::Get()) {
            return manager->findSubpass(renderTag);
        }
        return {};
    }



    //@―---------------------------------------------------------------------------
    //! @brief  グローバルマテリアルパラメータを設定
    //@―---------------------------------------------------------------------------
    void Material::SetGlobalFloat(StringView name, f32 value) {
        if (auto manager = MaterialManager::Get()) {
            manager->setFloat(name, value);
        }
    }
    //@―---------------------------------------------------------------------------
    //! @brief  グローバルマテリアルパラメータを設定
    //@―---------------------------------------------------------------------------
    void Material::SetGlobalColor(StringView name, Color value) {
        if (auto manager = MaterialManager::Get()) {
            manager->setColor(name, value);
        }
    }
    //@―---------------------------------------------------------------------------
    //! @brief  グローバルマテリアルパラメータを設定
    //@―---------------------------------------------------------------------------
    void Material::SetGlobalMatrix(StringView name, const Matrix& value) {
        if (auto manager = MaterialManager::Get()) {
            manager->setMatrix(name, value);
        }
    }
    //@―---------------------------------------------------------------------------
    //! @brief  グローバルマテリアルパラメータを設定
    //@―---------------------------------------------------------------------------
    void Material::SetGlobalTexture(StringView name, const Ref<Texture>& value) {
        if (auto manager = MaterialManager::Get()) {
            manager->setTexture(name, value);
        }
    }

}