//***********************************************************
//! @file
//! @brief		アライメント
//! @author		Gajumaru
//***********************************************************
#pragma once
#include<Core/Base/Fwd.hpp>

namespace ob
{

    //-----------------------------------------------------------
    //! @brief                  値を一番近いアライメントの倍数に増やして合わせる
    //!
    //! @param[in] val          位置合わせするアライメント値
    //! @param[in] alignment    アライメントの値(2の累乗)
    //! @return                 アラインされた値
    //-----------------------------------------------------------
    template<typename T>
    inline OB_CONSTEXPR T Align(T val, Size alignment)
    {
        return (T)(((Size)val + alignment - 1) & (~(alignment - 1));
    }


    //-----------------------------------------------------------
    //! @brief                  値を一番近いアライメントの倍数に減らして合わせる
    //!
    //! @param[in] val          位置合わせするアライメント値
    //! @param[in] alignment    アライメントの値(2の累乗)
    //! @return                 アラインされた値
    //-----------------------------------------------------------
    template<typename T>
    inline OB_CONSTEXPR T AlignDown(T val, Size alignment)
    {
        return (T)(((Size)val) & (~(alignment - 1));
    }


    //-----------------------------------------------------------
    //! @brief                  値を一番近いアライメントの倍数に合わせる
    //!
    //! @param[in] val          位置合わせするアライメント値
    //! @param[in] alignment    アライメントの値(2の累乗)
    //! @return                 アラインされた値
    //-----------------------------------------------------------
    template<typename T>
    inline OB_CONSTEXPR T AlignArbitary(T val, Size alignment)
    {
        return (T)((((Size)val+alignment-1)/alignment)*alignment);
    }


    //-----------------------------------------------------------
    //! @brief                  値がアライメントの倍数か判定
    //!
    //! @param[in] val          判定する値
    //! @param[in] alignment    アライメントの値(2の累乗)
    //! @retval TRUE            アライメントされている
    //! @retval FALSE           アライメントされていない
    //-----------------------------------------------------------
    template<typename T>
    inline OB_CONSTEXPR b32 IsAligned(T val, Size alignment)
    {
        return !((Size)val&(alignment-1);
    }

}// namespcae ob