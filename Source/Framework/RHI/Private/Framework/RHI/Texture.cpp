﻿//***********************************************************
//! @file
//! @brief		テクスチャ
//! @author		Gajumaru
//***********************************************************
#include <Framework/RHI/Texture.h>
#include <Framework/RHI/RHI.h>
#include <Framework/RHI/SystemResourceModule.h>

namespace ob::rhi {

    Ref<Texture> Texture::Preset(PresetTexture type) {
        if (auto instance = SystemResourceModule::Get()) {
            return instance->getPresetTexture(type);
        }
        return nullptr;
    }
    Ref<Texture> Texture::White() {
        return Preset(PresetTexture::White);
    }
    Ref<Texture> Texture::Gray() {
        return Preset(PresetTexture::Gray);
    }
    Ref<Texture> Texture::Black() {
        return Preset(PresetTexture::Black);
    }
    Ref<Texture> Texture::Normal() {
        return Preset(PresetTexture::Normal);
    }
    Ref<Texture> Texture::Check() {
        return Preset(PresetTexture::Check);
    }

    //@―---------------------------------------------------------------------------
    //! @brief  コンストラクタ
    //! 
    //! @param desc テクスチャ定義
    //! @param name オブジェクト名
    //@―---------------------------------------------------------------------------
    Ref<Texture> Texture::Create(const TextureDesc& desc) {
        if (auto rhi= RHI::Get()) {
            return rhi->createTexture(desc);
        }
        return nullptr;
    }

    //@―---------------------------------------------------------------------------
    //! @brief  コンストラクタ
    //! 
    //! @param desc テクスチャ定義
    //! @param name オブジェクト名
    //@―---------------------------------------------------------------------------
    Ref<Texture> Texture::Create(BlobView blob,StringView name){
        if (auto rhi= RHI::Get()) {
            return rhi->createTexture(blob, name);
        }
        return nullptr;
    }



    //@―---------------------------------------------------------------------------
    //! @brief  コンストラクタ
    //! 
    //! @param desc テクスチャ定義
    //@―---------------------------------------------------------------------------
    Ref<Texture> Texture::Create(Size size, Span<IntColor> colors) {
        if (auto rhi= RHI::Get()) {
            return rhi->createTexture(size, colors);
        }
        return nullptr;
    }


    //@―---------------------------------------------------------------------------
    //! @brief      パスからテクスチャを読み込み
    //! 
    //! @param path ファイルパス
    //@―---------------------------------------------------------------------------
    Ref<Texture> Texture::Load(const Path& path) {

        FileStream fs(path);
        if (fs) {
            Blob blob(fs.size());
            fs.read(blob.data(), blob.size());
            return Texture::Create(blob, path.string());
        }

        return nullptr;
    }

}