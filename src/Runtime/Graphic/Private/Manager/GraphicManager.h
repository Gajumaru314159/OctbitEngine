//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once

namespace ob {
    namespace graphic {

        class Texture2D;

        //@―---------------------------------------------------------------------------
        //! @brief  説明
        //@―---------------------------------------------------------------------------
        class GraphicManager:public RefObject{
        public:
            Ref<Texture2D> loadTexture2D(const StringView filePath);            // テクスチャファイルの読み込み
            Ref<Texture2D> loadTexture2DAsync(const StringView filePath);       // テクスチャファイルの非同期読み込み
        };

    }// namespace graphic
}// namespcae ob