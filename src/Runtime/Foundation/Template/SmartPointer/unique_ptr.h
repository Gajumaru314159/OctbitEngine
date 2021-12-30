//***********************************************************
//! @file
//! @brief		ユニーク・ポインタ
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <memory>
#include <Runtime/Foundation/Base/Common.h>
#include <Runtime/Foundation/Memory/System/MemorySystem.h>
#include <Runtime/Foundation/Template/type_traits.h>

namespace ob {

    //@―---------------------------------------------------------------------------
    //! @brief  ユニーク・ポインタ
    //@―---------------------------------------------------------------------------
    template<typename T, typename TDeleter = std::default_delete<T>>
    using unique_ptr = std::unique_ptr<T, TDeleter>;


    //@―---------------------------------------------------------------------------
    //! @brief  unique_ptrの初期化
    //@―---------------------------------------------------------------------------
    template<typename T, typename... Args>
    typename ob::enable_if_t<!ob::is_array_v<T>, ob::unique_ptr<T>> make_unique(Args&&...args) {
        auto pValue = new T(ob::forward(Args)(args)...);
        return ob::unique_ptr<T>(pValue);
    }

    //@―---------------------------------------------------------------------------
    //! @brief  unique_ptrの配列初期化
    //@―---------------------------------------------------------------------------
    template<typename T>
    typename ob::enable_if_t<ob::is_array_v<T>&& ob::extent_v<T> == 0, ob::unique_ptr<T>> make_unique(size_t size) {
        auto pValue = new typename ob::remove_extent<T>::type[size]();
        return ob::unique_ptr<T>(pValue);
    }

    // 引数付きの配列初期化を禁止
    //! @cond
    template<typename T, typename... Args>
    typename ob::enable_if_t<ob::is_array_v<T>&& ob::extent_v<T> != 0, ob::unique_ptr<T >> make_unique(Args&&...args) = delete;
    //! @endcond

#if defined(OB_DEBUG)

    //@―---------------------------------------------------------------------------
    //! @brief  unique_ptrの初期化(デバッグアロケート)
    //@―---------------------------------------------------------------------------
    template<typename T, typename... Args>
    typename ob::enable_if_t<!ob::is_array_v<T>, ob::unique_ptr<T>> make_unique_debug(Args&&...args) {
        auto pValue = new(ob::MemorySystem::GetDebugHeapAllocator()) T(ob::forward(Args)(args)...);
        return ob::unique_ptr<T>(pValue);
    }

    //@―---------------------------------------------------------------------------
    //! @brief  unique_ptrの配列初期化(デバッグアロケート)
    //@―---------------------------------------------------------------------------
    template<typename T>
    typename ob::enable_if_t<ob::is_array_v<T>&& ob::extent_v<T> == 0, ob::unique_ptr<T>> make_unique_debug(size_t size) {
        auto pValue = new(ob::MemorySystem::GetDebugHeapAllocator()) typename ob::remove_extent<T>::type[size]();
        return ob::unique_ptr<T>(pValue);
    }

    // 引数付きの配列初期化を禁止
    //! @cond
    template<typename T, typename... Args>
    typename ob::enable_if_t<ob::is_array_v<T>&& ob::extent_v<T> != 0, ob::unique_ptr<T >> make_unique_debug(Args&&...args) = delete;
    //! @endcond


#endif

}// namespcae ob