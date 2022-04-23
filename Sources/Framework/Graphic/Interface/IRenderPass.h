﻿//***********************************************************
//! @file
//! @brief		レンダーパス・インターフェイス
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphic/Interface/GraphicObject.h>

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
    //! @brief      レンダーパス・インターフェイス
    //@―---------------------------------------------------------------------------
    class IRenderPass :public GraphicObject {
    public:

        //@―---------------------------------------------------------------------------
        //! @brief  妥当な状態か
        //@―---------------------------------------------------------------------------
        virtual bool isValid()const = 0;


    protected:

        virtual ~IRenderPass() = default;

    };

}// namespace pb::graphic