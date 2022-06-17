//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once

namespace ob::input {

    using FName = UUID;

    //@―---------------------------------------------------------------------------
    //! @brief  説明
    //@―---------------------------------------------------------------------------
    class IInputHandler {
    public:

        virtual void onKeyDown(FName name, s32 id){};
        virtual void onKeyUp(FName name, s32 id) {};
        virtual void onKeyPressed(FName name, s32 id,bool isRepeat) {};
        virtual void onKeyReleased(FName name,s32 id, bool isRepeat){};
        virtual void onKeyDoublePressed(FName name, s32 id, bool isRepeat) {};
        virtual void onKeyDoubleReleased(FName name, s32 id, bool isRepeat) {};
        virtual void onScalerChanged(FName name, s32 id, f32 value) {};
        virtual void onVectorChanged(FName name, s32 id, Vec2 value) {};

    };






    //===============================================================
    // インライン関数
    //===============================================================
    //! @cond



    //! @endcond
}// namespcae ob