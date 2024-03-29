﻿//***********************************************************
//! @file
//! @brief		ノイズ
//! @author		Gajumaru
//***********************************************************
#pragma once

namespace ob {

    //@―---------------------------------------------------------------------------
    //! @brief  ノイズ
    //@―---------------------------------------------------------------------------
    class Noise {
    public:

        //@―---------------------------------------------------------------------------
        //! @brief  パーリンノイズ
        //@―---------------------------------------------------------------------------
        static f32 Perlin(f32 x, f32 y = defaultY(), f32 z = defaultZ());


        //@―---------------------------------------------------------------------------
        //! @brief  パーリンノイズ
        //! 
        //! @params octaves     何回重ね合わせるか
        //! @params persistence 詳細なノイズの強度
        //@―---------------------------------------------------------------------------
        static f32 PerlinOctave(s32 octaves,f32 persistence,f32 x, f32 y = defaultY(), f32 z = defaultZ());


    private:


        static constexpr f32 defaultY() {
            return 0.3983755219259f;
        }
        static constexpr f32 defaultZ() {
            return 0.6932752758284f;
        }


    };






    //===============================================================
    // インライン関数
    //===============================================================
    //! @cond



    //! @endcond
}// namespcae ob