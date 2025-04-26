//***********************************************************
//! @file
//! @brief		デスクリプタテーブル
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/GraphicObject.h>
#include <Framework/RHI/Forward.h>
#include <Framework/RHI/Types/BindlessHandle.h>
#include <Framework/RHI/Types/DescriptorTableDesc.h>

namespace ob::rhi {

    //! @brief      デスクリプタテーブル
    class DescriptorTable :public GraphicObject {
    public:

        //! @brief              コンストラクタ
        //!
        //! @param type         デスクリプタに設定するリソースの種類
        //! @param elementNum   要素数
        //! @param name         デバッグ名
        static Ref<DescriptorTable> Create(const DescriptorTableDesc& desc);

    public:

        virtual const DescriptorTableDesc& getDesc() const = 0;

        //! @brief  リソースを設定
        //! @{
        virtual bool setResource(s32 index, const Ref<Buffer>& resource) = 0;
        virtual bool setResource(s32 index, const Ref<Texture>& resource) = 0;
        virtual bool setResource(s32 index, const Ref<Sampler>& resource) = 0;
        //! @}


        //! @brief  CPUハンドル取得
        virtual BindlessHandle getBindlessHandle(s32 index = 0)const = 0;

    };

}