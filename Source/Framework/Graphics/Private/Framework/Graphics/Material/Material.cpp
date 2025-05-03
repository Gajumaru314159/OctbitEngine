//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include <Framework/Graphics/Material/Material.h>
#include <Framework/Graphics/Material/MaterialImpl.h>
#include <Framework/Graphics/Material/MaterialManager.h>
#include <Framework/Graphics/Material/MaterialSystem.h>
#include <Framework/RHI/Sampler.h>

namespace ob::graphics {

    //! @brief  生成
    Ref<Material> Material::Create(const MaterialDesc& desc) {
        return new MaterialImpl(desc);
    }


    //! @brief  グローバルマテリアルパラメータを設定
    void Material::SetGlobalFloat(StringView name, f32 value) {
        if (auto system = MaterialSystem::Get()) {
            system->getGlobalBlock().setScalar(name, value);
        }
    }
    //! @brief  グローバルマテリアルパラメータを設定
    void Material::SetGlobalColor(StringView name, Color value) {
        if (auto system = MaterialSystem::Get()) {
            system->getGlobalBlock().setVector(name, value);
        }
    }
    //! @brief  グローバルマテリアルパラメータを設定
    void Material::SetGlobalMatrix(StringView name, const Matrix& value) {
        if (auto system = MaterialSystem::Get()) {
            system->getGlobalBlock().setMatrix(name, value);
        }
    }
    //! @brief  グローバルマテリアルパラメータを設定
    void Material::SetGlobalTexture(StringView name, const Ref<Texture>& value) {
        if (auto system = MaterialSystem::Get()) {
            system->getGlobalBlock().setTexture(name, value,rhi::Sampler::Default());
        }
    }

}