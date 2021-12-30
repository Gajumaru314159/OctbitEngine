﻿//***********************************************************
//! @file
//! @brief		時間/日時型
//! @author		Gajumaru
//***********************************************************
#pragma once
namespace ob {
    namespace platform {


        //@―---------------------------------------------------------------------------
        //! @brief  曜日
        //@―---------------------------------------------------------------------------
        enum class DayOfWeek {
            Sunday,     //!< 日曜日
            Monday,     //!< 月曜日 
            Tuesday,    //!< 火曜日
            Wednesday,  //!< 水曜日
            Thursday,   //!< 木曜日
            Friday,     //!< 金曜日
            Saturday,   //!< 土曜日
        };


        //@―---------------------------------------------------------------------------
        //! @brief  日時型
        //@―---------------------------------------------------------------------------
        struct Date {
            s32 year;
            s32 month;
            DayOfWeek dayOfWeek;
            s32 day;
            s32 hour;
            s32 minute;
            s32 second;
            s32 milliseconds;
        };

    }// namespace platform
}// namespcae ob