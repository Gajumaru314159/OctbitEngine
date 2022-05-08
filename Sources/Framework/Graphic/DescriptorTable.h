﻿//***********************************************************
//! @file
//! @brief		デスクリプタ・テーブル
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphic/Utility/GraphicObjectHolder.h>
#include <Framework/Graphic/Types/DescriptorDesc.h>

namespace ob::graphic {

    //@―---------------------------------------------------------------------------
    //! @brief  説明
    //@―---------------------------------------------------------------------------
    class DescriptorTable {
        OB_DEFINE_GRAPHIC_OBJECT_HOLDER(DescriptorTable);
    public:

        //===============================================================
        // コンストラクタ / デストラクタ
        //===============================================================

        DescriptorTable() = default;

        //@―---------------------------------------------------------------------------
        //! @brief              コンストラクタ
        //!
        //! @param type         デスクリプタに設定するリソースの種類
        //! @param elementNum   要素数
        //! @param name         デバッグ名
        //@―---------------------------------------------------------------------------
        DescriptorTable(DescriptorHeapType type, s32 elementNum, StringView name = TC("DescriptorTable"));


        //@―---------------------------------------------------------------------------
        //! @brief  リソースを設定
        //@―---------------------------------------------------------------------------
        //! @{
        bool setResource(s32 index, class Buffer& resource);  // CBV / UAV
        bool setResource(s32 index, class Texture& resource); // SRV / RTV / DSV
        bool setResource(s32 index, class Sampler& resource); // Sampler
        //! @}

        
        //@―---------------------------------------------------------------------------
        //! @brief  リソースのバインドを解除
        //@―---------------------------------------------------------------------------
        void clear();


        //@―---------------------------------------------------------------------------
        //! @brief  指定したインデックスのリソースのバインドを解除
        //@―---------------------------------------------------------------------------
        void clearAt(s32 index);

    private:



    };


    //@―---------------------------------------------------------------------------
    //! @brief  説明
    //@―---------------------------------------------------------------------------
    class Descriptor {
    public:

        //===============================================================
        // コンストラクタ / デストラクタ
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief  説明
        //@―---------------------------------------------------------------------------
        bool setResource(class Buffer& buffer);  // CBV / UAV
        bool setResource(class Texture& buffer); // SRV / RTV / DSV
        bool setResource(class Sampler& buffer); // Sampler

        void clear();

    private:



    };

}// namespcae ob::graphic