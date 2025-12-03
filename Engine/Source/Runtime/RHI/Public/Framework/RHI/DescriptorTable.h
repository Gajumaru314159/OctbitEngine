//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/GraphicObject.h>
#include <Framework/RHI/Forward.h>
#include <Framework/RHI/Types/DescriptorTableDesc.h>

namespace ob::rhi {

    //! @brief      デスクリプタテーブル
    class DescriptorTable :public GraphicObject {
    public:

        //! @brief              コンストラクタ
        static Ref<DescriptorTable> Create(const DescriptorTableDesc& desc);

    public:

        virtual const DescriptorTableDesc& getDesc() const = 0;

        //! @brief  リソースを設定
        //! @{
        virtual bool setResource(s32 index, const Ref<Buffer>& resource) = 0;
        virtual bool setResource(s32 index, const Ref<Texture>& resource) = 0;
        virtual bool setResource(s32 index, const Ref<Sampler>& resource) = 0;
        //! @}

    };

}