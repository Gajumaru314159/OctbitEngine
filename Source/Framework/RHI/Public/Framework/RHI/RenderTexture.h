﻿//***********************************************************
//! @file
//! @brief		レンダー・ターゲット
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/Utility/GraphicObjectHolder.h>
#include <Framework/RHI/Types/RenderTextureDesc.h>
#include <Framework/RHI/Texture.h>

//===============================================================
// 前方宣言
//===============================================================
namespace ob::rhi {
    class Texture;
}


//===============================================================
// クラス宣言
//===============================================================
namespace ob::rhi {

    //@―---------------------------------------------------------------------------
    //! @brief  レンダー・ターゲット
    //@―---------------------------------------------------------------------------
    class RenderTexture : public Texture {
        OB_DEFINE_GRAPHIC_OBJECT_HOLDER(RenderTexture);
    public:

        //===============================================================
        // コンストラクタ / デストラクタ
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief  コンストラクタ
        //@―---------------------------------------------------------------------------
        RenderTexture()=default;


        //@―---------------------------------------------------------------------------
        //! @brief  コンストラクタ
        //! 
        //! @param desc     定義
        //! @param name     オブジェクト名
        //@―---------------------------------------------------------------------------
        RenderTexture(const RenderTextureDesc& desc);

    };

}// namespcae ob::rhi