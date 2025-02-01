//***********************************************************
//! @file
//! @brief		ルートシグネチャ
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/GraphicObject.h>
#include <Framework/RHI/Types/RootSignatureDesc.h>

namespace ob::rhi {

    //! @brief      ルートシグネチャ
    class IRootSignature :public GraphicObject {
    public:
        
        //! @brief      定義を取得
        virtual const RootSignatureDesc& getDesc()const = 0;

    };

}