//***********************************************************
//! @file
//! @brief		テクスチャ
//! @author		Gajumaru
//***********************************************************
#include <Framework/RHI/Texture.h>
#include <Framework/RHI/Device.h>

namespace ob::rhi {

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
    Ref<Texture> Texture::Create(BlobView blob){
        return Device::Get()->createTexture(blob);
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
            return Texture::Create(blob);
        }

        return nullptr;
    }

}// namespace ob::rhi