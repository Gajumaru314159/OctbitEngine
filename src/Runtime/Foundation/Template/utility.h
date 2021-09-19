//***********************************************************
//! @file
//! @brief		ユーティリティ
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <utility>
#include <Foundation/Base/PrimitiveType.h>
#include <Foundation/Template/type_traits.h>

namespace ob
{

    using std::swap;
    using std::exchange;

    using std::forward;
    using std::move;
    using std::move_if_noexcept;

    using std::as_const;

    using std::declval;

    using std::pair;
    using std::make_pair;
    using std::in_place_t;
    using std::in_place;
    using std::in_place_type_t;
    using std::in_place_type;
    using std::in_place_index_t;
    using std::in_place_index;

    using std::integer_sequence;
    using std::make_integer_sequence;
    using std::index_sequence;
    using std::make_index_sequence;
    using std::index_sequence_for;


    //@―-------------------------------------------------------------------------------
    //! @brief      enum値を基底型(整数型)に変換する
    //! 
    //! @param e    enum値
    //@―-------------------------------------------------------------------------------
    template<typename T>
    inline constexpr enable_if_t<is_enum_v<T>,underlying_type_t<T>> enum_cast(T e)
    {
        return static_cast<underlying_type_t<T>>(e);
    }


    //@―-------------------------------------------------------------------------------
    //! @brief      コンテナのサイズを取得
    //@―-------------------------------------------------------------------------------
    template<typename T>
    inline constexpr s32 get_size(const T& container)noexcept(noexcept(container.size()))
    {
        return static_cast<s32>(container.size());
    }


    //@―-------------------------------------------------------------------------------
    //! @brief      静的配列のサイズを取得
    //@―-------------------------------------------------------------------------------
    template<typename T, s32 N>
    inline constexpr s32 get_size(const T(&)[N])noexcept
    {
        return N;
    }

}// namespcae ob