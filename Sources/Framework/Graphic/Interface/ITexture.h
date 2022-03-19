//***********************************************************
//! @file
//! @brief		テクスチャ・インターフェイス
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphic/Interface/GraphicObject.h>
#include <Framework/Graphic/Types/TextureFormat.h>
#include <Framework/Graphic/Types/ResourceUsage.h>

//===============================================================
// 前方宣言
//===============================================================
namespace ob::graphic {
}

//===============================================================
// クラス宣言
//===============================================================
namespace ob::graphic {

    //@―---------------------------------------------------------------------------
    //! @brief      テクスチャ・インターフェイス
    //@―---------------------------------------------------------------------------
    class ITexture :public GraphicObject {
    public:

        virtual auto getDesc()const -> const TextureDesc & = 0;

    };

}// namespace pb::graphic