//***********************************************************
//! @file
//! @brief		ユニーク・ポインタ
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <memory>
#include <Foundation/Base/Fwd.h>
#include <Foundation/Memory/Device/MemoryDevice.h>
#include <Foundation/Template/type_traits.h>

namespace ob
{

    //@―-------------------------------------------------------------------------------
    template<typename T,typename TDeleter=std::default_delete<T>>
    using unique_ptr = std::unique_ptr<T, TDeleter>;

    template<typename T, typename... Args>
    typename enable_if<!is_array<T>::value, unique_ptr<T>>::type make_unique(Args&&...args)
    {
        auto pValue = new(MemoryDevice::GetHeap()) T(forward<Args>(args)...);
        return unique_ptr<T>(pValue);
    }

    //template<typename T>
    //typename enable_if<conjunction<is_array<T>::value,extent<T>::value>::value==0, unique_ptr<T>>::type make_unique(size_t size)
    //{
    //    auto pValue = new(MemoryDevice::GetHeap()) typename remove_extent<T>::type[size]();
    //    return unique_ptr<T>(pValue);
    //}

    //template<typename T>
    //typename enable_if<conjunction<is_array<T>::value, extent<T>::value>::value != 0, unique_ptr<T>>::type make_unique(Args&&... args) = delete;

#if defined(OB_DEBUG)

    template<typename T, typename... Args>
    typename enable_if<!is_array<T>::value, unique_ptr<T>>::type make_unique_debug(Args&&...args)
    {
        auto pValue = new(MemoryDevice::GetDebugHeap()) T(forward<Args>(args)...);
        return unique_ptr<T>(pValue);
    }

    //template<typename T>
    //typename enable_if<conjunction<is_array<T>::value, extent<T>::value>::value == 0, unique_ptr<T>>::type make_unique_debug(size_t size)
    //{
    //    auto pValue = new(MemoryDevice::GetDebugHeap()) typename remove_extent<T>::type[size]();
    //    return unique_ptr<T>(pValue);
    //}

    //template<typename T>
    //typename enable_if<conjunction<is_array<T>::value, extent<T>::value>::value != 0, unique_ptr<T>>::type make_unique_debug(Args&&... args) = delete;

#endif

}// namespcae ob