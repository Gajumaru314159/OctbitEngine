﻿//***********************************************************
//! @file
//! @brief		レンダーパス・インターフェイス
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/GraphicObject.h>
#include <Framework/RHI/Types/RenderPassDesc.h>

//===============================================================
// 前方宣言
//===============================================================
namespace ob::rhi {
}

//===============================================================
// クラス宣言
//===============================================================
namespace ob::rhi {

    //@―---------------------------------------------------------------------------
    //! @brief      レンダーパス・インターフェイス
    //@―---------------------------------------------------------------------------
    class IRenderPass :public GraphicObject {
    public:

		//@―---------------------------------------------------------------------------
		//! @brief  妥当な状態か
		//@―---------------------------------------------------------------------------
		virtual bool isValid()const = 0;
            
    };

}// namespace pb::rhi