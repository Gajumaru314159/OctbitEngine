//***********************************************************
//! @file
//! @brief		デスクリプタテーブル
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/GraphicObject.h>
#include <Framework/RHI/Types/DescriptorDesc.h>

namespace ob::rhi {

    //@―---------------------------------------------------------------------------
    //! @brief      デスクリプタテーブル
    //@―---------------------------------------------------------------------------
    class DescriptorTable :public GraphicObject {
    public:

        //@―---------------------------------------------------------------------------
        //! @brief              コンストラクタ
        //!
        //! @param type         デスクリプタに設定するリソースの種類
        //! @param elementNum   要素数
        //! @param name         デバッグ名
        //@―---------------------------------------------------------------------------
        static Ref<DescriptorTable> Create(DescriptorHeapType type, s32 elementNum);

    public:

        //@―---------------------------------------------------------------------------
        //! @brief  リソースを設定
        //@―---------------------------------------------------------------------------
        //! @{
        virtual bool setResource(s32 index, const Ref<class Buffer>& resource) = 0;
        virtual bool setResource(s32 index, const Ref<class Texture>& resource) = 0;
        //virtual bool setResource(s32 index, class Sampler& resource) = 0;
        //! @}

    };

}