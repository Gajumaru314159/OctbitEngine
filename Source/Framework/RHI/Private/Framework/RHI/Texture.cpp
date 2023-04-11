//***********************************************************
//! @file
//! @brief		テクスチャ
//! @author		Gajumaru
//***********************************************************
#include <Framework/RHI/Texture.h>
#include <Framework/RHI/Device.h>
#include <Framework/RHI/SystemResourceModule.h>

namespace ob::rhi {

    Ref<Texture> Texture::Preset(PresetTexture type) {
        // 高速取得のためキャッシュ
        static std::atomic<SystemResourceModule*> pModule = nullptr;
        if (pModule == nullptr) {
            pModule = GEngine->get2<SystemResourceModule>();
        }

        if (pModule) {
            return pModule.load()->getPresetTexture(type);
        } else {
            LOG_ERROR("ob::rhi::Systemが初期化されていないためデバイスの取得に失敗しました。");
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
        if (auto device = Device::Get()) {
            return device->createTexture(desc);
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
        if (auto device = Device::Get()) {
            return device->createTexture(blob, name);
        }
        return nullptr;
    }



    //@―---------------------------------------------------------------------------
    //! @brief  コンストラクタ
    //! 
    //! @param desc テクスチャ定義
    //@―---------------------------------------------------------------------------
    Ref<Texture> Texture::Create(Size size, Span<IntColor> colors) {
        if (auto device = Device::Get()) {
            return device->createTexture(size, colors);
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

}// namespace ob::rhi