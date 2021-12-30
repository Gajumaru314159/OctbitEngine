﻿//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include "GraphicManager.h"
#include <Runtime/Graphic/Public/Texture/Texture2D.h>

namespace ob {
    namespace graphic {


        //@―---------------------------------------------------------------------------
        //! @brief  テクスチャファイルの読み込み
        //@―---------------------------------------------------------------------------
        Ref<Texture2D> GraphicManager::loadTexture2D(const StringView filePath) {
            auto texture = MakeRef<Texture2D>();
        }


        //@―---------------------------------------------------------------------------
        //! @brief  テクスチャファイルの非同期読み込み
        //@―---------------------------------------------------------------------------
        Ref<Texture2D> GraphicManager::loadTexture2DAsync(const StringView filePath) {

        }

    }// namespace graphic
}// namespace ob