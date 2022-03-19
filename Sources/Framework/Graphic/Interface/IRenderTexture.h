//***********************************************************
//! @file
//! @brief		レンダー・テクスチャ・インターフェイス
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
    //! @brief      レンダー・テクスチャ・インターフェイス
    //@―---------------------------------------------------------------------------
    class IRenderTexture :public GraphicObject {
    public:

        virtual ITexture* getTexture(s32 index = 0) = 0;
        virtual ITexture* getDepthStencilTexture(s32 index = 0) = 0;

    protected:

        virtual ~IRenderTexture()=default;


    };

}// namespace pb::graphic