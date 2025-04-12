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
    class RootSignature :public GraphicObject {
    public:
        
        //! @brief  生成
        static Ref<RootSignature> Create(const BindingLayoutDesc& desc);

    public:

        //! @brief      定義を取得
        virtual const BindingLayoutDesc& getDesc()const = 0;

    };

}