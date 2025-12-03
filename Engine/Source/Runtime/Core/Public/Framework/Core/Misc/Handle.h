//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/CorePrivate.h>

namespace ob::core {

#define OB_DEFINE_HANDLE_WITH_BASE(type,base) \
    struct Handle_##type {};\
    using type = ob::core::Handle<base,Handle_##type>

#define OB_DEFINE_HANDLE(type) OB_DEFINE_HANDLE_WITH_BASE(type,ob::core::s32)


    template<class T = s32, class TNamespace = void>
    class Handle {
    public:
        using IndexType = T;
        static_assert(std::is_integral_v<T>, "Integral type required for Handle<T>");

        static constexpr T NullIndex = T(-1);

        constexpr Handle() = default;
        constexpr explicit Handle(T index) : m_index{index} {}

        constexpr bool operator == (const Handle& rhs) const;
        constexpr bool operator != (const Handle& rhs) const;
        constexpr bool operator < (const Handle& rhs) const;
        constexpr bool operator > (const Handle& rhs) const;
        constexpr bool operator <= (const Handle& rhs) const;

        constexpr operator bool () const;

        void reset();

        constexpr T index() const;

        constexpr bool empty() const;

    private:
        T m_index = NullIndex;
    };

    template <typename T, typename TNamespace>
    constexpr bool Handle<T, TNamespace>::operator==(const Handle& rhs) const {
        return m_index == rhs.m_index;
    }

    template <typename T, typename TNamespace>
    constexpr bool Handle<T, TNamespace>::operator!=(const Handle& rhs) const {
        return m_index != rhs.m_index;
    }

    template <typename T, typename TNamespace>
    constexpr bool Handle<T, TNamespace>::operator<(const Handle& rhs) const {
        return m_index < rhs.m_index;
    }

    template <typename T, typename TNamespace>
    constexpr bool Handle<T, TNamespace>::operator<=(const Handle& rhs) const {
        return m_index <= rhs.m_index;
    }

    template <typename T, typename TNamespace>
    constexpr bool Handle<T, TNamespace>::operator>(const Handle& rhs) const {
        return m_index > rhs.m_index;
    }

    template <class T, class TNamespace>
    constexpr Handle<T, TNamespace>::operator bool() const     {
        return m_index != NullIndex;
    }

    template <typename T, typename TNamespace>
    void Handle<T, TNamespace>::reset() {
        m_index = NullIndex;
    }

    template <typename T, typename TNamespace>
    constexpr T Handle<T, TNamespace>::index() const {
        return m_index;
    }

    template <typename T, typename TNamespace>
    constexpr bool Handle<T, TNamespace>::empty() const {
        return m_index == NullIndex;
    }

}

template<class T>
struct std::hash<ob::core::Handle<T>>
{
    constexpr size_t operator()(const ob::core::Handle<T>& value)const noexcept {
        size_t result = 0;
        ob::core::Hash::Combine(result,value.index());
        return result;
    }

};