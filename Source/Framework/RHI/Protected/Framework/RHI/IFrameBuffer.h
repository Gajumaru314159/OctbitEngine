﻿//***********************************************************
//! @file
//! @brief		フレームバッファ・インターフェイス
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/GraphicObject.h>
#include <Framework/RHI/Types/FrameBufferDesc.h>

//===============================================================
// クラス宣言
//===============================================================
namespace ob::rhi {

    //@―---------------------------------------------------------------------------
    //! @brief      フレームバッファ・インターフェイス
    //@―---------------------------------------------------------------------------
    class IFrameBuffer :public GraphicObject {
    public:

        //@―---------------------------------------------------------------------------
        //! @brief  妥当な状態か
        //@―---------------------------------------------------------------------------
        virtual bool isValid()const = 0;

        //@―---------------------------------------------------------------------------
        //! @brief  デスクリプタ取得
        //@―---------------------------------------------------------------------------
        virtual const FrameBufferDesc& desc()const = 0;
            
    };

}// namespace pb::rhi