//***********************************************************
//! @file
//! @brief		デスクリプタテーブル
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/GraphicObject.h>
#include <Framework/RHI/Forward.h>
#include <Framework/RHI/Types/DescriptorDesc.h>

namespace ob::rhi {

    //! @brief      デスクリプタテーブル
    class IDescriptorTable :public GraphicObject {
    public:

        //! @brief  リソースを設定
        //! @{
        virtual bool setResource(s32 index, const Buffer& resource) = 0;
        virtual bool setResource(s32 index, const Texture& resource) = 0;
        //virtual bool setResource(s32 index, class Sampler& resource) = 0;
        //! @}

    };

}