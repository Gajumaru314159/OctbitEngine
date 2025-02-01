//***********************************************************
//! @file
//! @brief		テクスチャ
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/Forward.h>
#include <Framework/RHI/GraphicObject.h>
#include <Framework/RHI/Texture.h>

namespace ob::rhi {

    //! @brief      テクスチャ
    class ITexture :public GraphicObject {
    public:

        //! @brief      名前を設定
        virtual void setName(StringView) = 0;

        //! @brief      定義取得
        virtual const TextureDesc& desc()const = 0;

        //! @brief      イベントリスナ追加
        //! @details    テクスチャリサイズ時に呼ばれます。
        virtual void addEventListener(TextureEventHandle& handle, TextureEventDelegate func) = 0;

    };

}