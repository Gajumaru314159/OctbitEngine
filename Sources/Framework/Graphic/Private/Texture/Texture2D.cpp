//***********************************************************
//! @file
//! @brief		テクスチャ
//! @author		Gajumaru
//***********************************************************
#include <Framework/Graphic/Public/Texture/Texture2D.h>

namespace ob {
    namespace graphic {

        //@―---------------------------------------------------------------------------
        //! @brief  テクスチャの生成
        //@―---------------------------------------------------------------------------
        Ref<Texture2D> Texture2D::create(s32 width, s32 height) {
            Ref<Texture2D> texture = MakeRef<Texture2D>();
            texture->m_object;
            return texture;
        }


        //@―---------------------------------------------------------------------------
        //! @brief  テクスチャの生成
        //@―---------------------------------------------------------------------------
        Ref<Texture2D> Texture2D::create(s32 width, s32 height, TextureFormat format) {

        }


        //@―---------------------------------------------------------------------------
        //! @brief  ファイルからテクスチャを読み込み
        //@―---------------------------------------------------------------------------
        Ref<Texture2D> Texture2D::load(const StringView& filePath) {

        }


        //@―---------------------------------------------------------------------------
        //! @brief  ファイルからテクスチャを非同期読み込み
        //@―---------------------------------------------------------------------------
        Ref<Texture2D> Texture2D::loadAsync(const StringView& filePath) {

        }


        Texture2D::Texture2D(s32 width, s32 height, TextureFormat format){
            width = get_max(1, width);
            height = get_max(1, height);

        }

    }// namespace graphic
}// namespace ob