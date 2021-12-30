﻿//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once
#include "Texture.h"
namespace ob {
    namespace graphic {

        namespace internal {
            //@―---------------------------------------------------------------------------
            //! @brief  説明
            //@―---------------------------------------------------------------------------
            class IImageTexture {
            public:

                //===============================================================
                // コンストラクタ / デストラクタ
                //===============================================================

                //@―---------------------------------------------------------------------------
                //! @brief  説明
                //@―---------------------------------------------------------------------------
                virtual bool isLoaded()const;


            private:



            };
        }// namespace internal



    }// namespace graphic
}// namespcae ob