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
        static SystemResourceModule* pModule = nullptr;
        if (pModule == nullptr) {
            pModule = GEngine->get<SystemResourceModule>();
        }
        OB_ASSERT(pModule, "ob::rhi::Systemが初期化されていないためデバイスの取得に失敗しました。");
        return pModule->getPresetTexture(type);
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
        return Device::Get()->createTexture(desc);
    }

    //@―---------------------------------------------------------------------------
    //! @brief  コンストラクタ
    //! 
    //! @param desc テクスチャ定義
    //! @param name オブジェクト名
    //@―---------------------------------------------------------------------------
    Ref<Texture> Texture::Create(BlobView blob,StringView name){
        return Device::Get()->createTexture(blob,name);
    }



    //@―---------------------------------------------------------------------------
    //! @brief  コンストラクタ
    //! 
    //! @param desc テクスチャ定義
    //@―---------------------------------------------------------------------------
    Ref<Texture> Texture::Create(Size size, Span<IntColor> colors) {
        return Device::Get()->createTexture(size, colors);
    }


    //@―---------------------------------------------------------------------------
    //! @brief      パスからテクスチャを読み込み
    //! 
    //! @param path ファイルパス
    //@―---------------------------------------------------------------------------
    Ref<Texture> Texture::Load(const Path& path) {

        FileStream fs(path);
        if (fs) {

            String name;
            StringEncoder::Encode(path.native(),name);

            Blob blob(fs.size());
            fs.read(blob.data(), blob.size());
            return Texture::Create(blob,name);
        }

        return nullptr;
    }

}// namespace ob::rhi