//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Runtime/Core/Type/Size.h>
#include <Runtime/Core/Graphic/Color.h>

#include "PixelFormat.h"

namespace ob {
    namespace graphic {

        //@―---------------------------------------------------------------------------
        //! @brief  説明
        //@―---------------------------------------------------------------------------
        class Bitmap{
        public:

            //===============================================================
            // コンストラクタ / デストラクタ
            //===============================================================

            //@―---------------------------------------------------------------------------
            //! @brief  説明
            //@―---------------------------------------------------------------------------
            Bitmap();
            explicit Bitmap(Size size, Color color = Color::white);
            explicit Bitmap(StringView path, PixelFormtat format= PixelFormtat::Auto);

            Size getSize()const noexcept;

            Color getPixel(s32 x, s32 y, s32 z);
            void setPixel(s32 x, s32 y,s32 z,Color& color);

        private:

            Size         m_size;
            vector<byte> m_data;

        };

    }// namespace graphic
}// namespcae ob