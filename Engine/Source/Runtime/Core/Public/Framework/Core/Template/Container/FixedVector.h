#pragma once
#include <array>
#include <stdexcept>
#include <iterator>
#include <algorithm>
#include <initializer_list>
#include <utility>
#include <Framework/Core/Template/Utility/TypeTraits.h>

namespace ob::core {

    //! @brief  Fixed配列
    template<typename T, std::size_t N>
    class FixedVector {
    public:
        using reference = T&;
        using const_reference = const T&;
        using iterator = typename std::array<T, N>::iterator;
        using const_iterator = typename std::array<T, N>::const_iterator;
        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;
        using value_type = T;
        using pointer = value_type*;
        using const_pointer = const value_type*;
        using reverse_iterator = typename std::array<T, N>::reverse_iterator;
        using const_reverse_iterator = typename std::array<T, N>::const_reverse_iterator;
    public:

        //===============================================================
        // コンストラクタ / デストラクタ
        //===============================================================
        constexpr FixedVector() noexcept : m_size(0) {}

        constexpr FixedVector(size_type size)
            : m_size(size) 
        {
            if (size > N) throw std::overflow_error("FixedVector: overflow");
        }

        constexpr FixedVector(size_type size, const T& value)
            : m_size(size) 
        {
            if (size > N) throw std::overflow_error("FixedVector: overflow");
            for (s32 i = 0; i < m_size; ++i) {
                m_data[i] = value;
            }
        }

        constexpr FixedVector(std::initializer_list<T> init)
            : m_size(init.size())
        {
            if (init.size() > N) throw std::overflow_error("FixedVector: overflow");
            for (s32 i = 0; i < m_size; ++i) {
				m_data[i] = *(init.begin() + i);
            }
        }

        template<class InputIt,std::enable_if_t<is_iterator<InputIt>::value>>
        constexpr FixedVector(InputIt first, InputIt last) {
            size_type count = std::distance(first, last);
            if (count > N) throw std::overflow_error("FixedVector: overflow");
			for (auto itr = first; first != last; ++itr) {
				m_data[m_size++] = *itr;
			}
        }


        //===============================================================
        // イテレータ
        //===============================================================

        constexpr iterator begin() noexcept {
            return m_data.begin();
        }

        constexpr const_iterator begin() const noexcept {
            return m_data.begin();
        }

        constexpr const_iterator cbegin() const noexcept {
            return m_data.cbegin();
        }

        constexpr iterator end() noexcept {
            return m_data.begin() + m_size;
        }

        constexpr const_iterator end() const noexcept {
            return m_data.begin() + m_size;
        }

        constexpr const_iterator cend() const noexcept {
            return m_data.cbegin() + m_size;
        }

        constexpr reverse_iterator rbegin() noexcept {
            return m_data.rbegin();
        }

        constexpr const_reverse_iterator rbegin() const noexcept {
            return m_data.rbegin();
        }

        constexpr const_reverse_iterator crbegin() const noexcept {
            return m_data.crbegin();
        }

        constexpr reverse_iterator rend() noexcept {
            return m_data.rbegin() + m_size;
        }

        constexpr const_reverse_iterator rend() const noexcept {
            return m_data.rbegin() + m_size;
        }

        constexpr const_reverse_iterator crend() const noexcept {
            return m_data.crbegin() + m_size;
        }


        //===============================================================
        // 領域
        //===============================================================

        constexpr size_type size() const noexcept {
            return m_size;
        }

        static constexpr size_type max_size() noexcept {
            return N;
        }

        constexpr void resize(size_type count) {
            if (count > N) throw std::overflow_error("FixedVector: overflow");
            m_size = count;
        }

        constexpr void resize(size_type count, const value_type& value) {
            if (count > N) throw std::overflow_error("FixedVector: overflow");
            if (count > m_size) {
                for (s32 i = 0; i < count; ++i) {
					m_data[i] = value;
                }
            }
            m_size = count;
        }

        static constexpr size_type capacity() noexcept {
            return N;
        }

        constexpr bool empty() const noexcept {
            return m_size == 0;
        }


        //===============================================================
        // 要素アクセス
        //===============================================================

        constexpr reference operator[](size_type pos) {
            return m_data[pos];
        }

        constexpr const_reference operator[](size_type pos) const {
            return m_data[pos];
        }

        constexpr reference at(size_type pos) {
            if (pos >= m_size) throw std::out_of_range("FixedVector: out of range");
            return m_data[pos];
        }

        constexpr const_reference at(size_type pos) const {
            if (pos >= m_size) throw std::out_of_range("FixedVector: out of range");
            return m_data[pos];
        }

        constexpr pointer data() {
            return m_data.data();
        }
        
        constexpr const_pointer data() const {
            return m_data.data();
        }

        constexpr reference front() {
            return m_data.front();
        }

        constexpr const_reference front() const {
            return m_data.front();
        }

        constexpr reference back() {
            return m_data[m_size - 1];
        }

        constexpr const_reference back() const {
            return m_data[m_size - 1];
        }


        //===============================================================
        // コンテナの変更
        //===============================================================

        constexpr void assign(size_type n, const T& t) {
            for (s32 i = 0; i < n; ++i) {
				m_data[i] = t;
            }
        }

        template<class InputIt, std::enable_if_t<is_iterator<InputIt>::value>>
        constexpr void assign(InputIt first, InputIt last) {
            size_type count = std::distance(first, last);
            if (count > N) throw std::overflow_error("FixedVector: overflow");
            s32 i = 0;
            for (auto itr = first; first != last; ++itr) {
                m_data[i++] = *itr;
            }
            m_size = count;
        }

        constexpr void push_back(const T& value) {
            if (m_size >= N) throw std::overflow_error("FixedVector: overflow");
            m_data[m_size++] = value;
        }

        constexpr void push_back(T&& value) {
            if (m_size >= N) throw std::overflow_error("FixedVector: overflow");
            m_data[m_size++] = std::move(value);
        }

        template<class... Args>
        constexpr reference emplace_back(Args&&... args) {
            if (m_size >= N) throw std::overflow_error("FixedVector: overflow");
            m_data[m_size] = T(std::forward<Args>(args)...);
            return m_data[m_size++];
        }

        constexpr void pop_back() {
            if (m_size > 0) --m_size;
        }

        constexpr iterator insert(const_iterator pos, const T& value) {
            return insert_impl(pos, value);
        }

        constexpr iterator insert(const_iterator pos, T&& value) {
            return insert_impl(pos, std::move(value));
        }

        constexpr iterator insert(const_iterator pos, size_type count, const T& value) {
            if (m_size + count > N) throw std::overflow_error("FixedVector: overflow");
            auto index = pos - cbegin();
            if (count > 0) {
                std::move_backward(begin() + index, end(), end() + count);
                std::fill(begin() + index, begin() + index + count, value);
                m_size += count;
            }
            return begin() + index;
        }

        template<class InputIt, std::enable_if_t<is_iterator<InputIt>::value>>
        constexpr iterator insert(const_iterator pos, InputIt first, InputIt last) {
            size_type count = std::distance(first, last);
            if (m_size + count > N) throw std::overflow_error("FixedVector: overflow");
            auto index = pos - cbegin();
            if (count > 0) {
                std::move_backward(begin() + index, end(), end() + count);
                std::copy(first, last, begin() + index);
                m_size += count;
            }
            return begin() + index;
        }

        constexpr iterator insert(const_iterator pos, std::initializer_list<T> ilist) {
            return insert(pos, ilist.begin(), ilist.end());
        }

        template<class... Args>
        constexpr iterator emplace(const_iterator pos, Args&&... args) {
            if (m_size >= N) throw std::overflow_error("FixedVector: overflow");
            auto index = pos - cbegin();
            if (index < m_size) {
                std::move_backward(begin() + index, end(), end() + 1);
            }
            m_data[index] = T(std::forward<Args>(args)...);
            ++m_size;
            return begin() + index;
        }

        constexpr iterator erase(const_iterator pos) {
            auto index = pos - cbegin();
            if (index < m_size) {
                std::move(begin() + index + 1, end(), begin() + index);
                --m_size;
            }
            return begin() + index;
        }

        constexpr iterator erase(const_iterator first, const_iterator last) {
            auto start_index = first - cbegin();
            auto end_index = last - cbegin();
            if (start_index < end_index && end_index <= m_size) {
                std::move(begin() + end_index, end(), begin() + start_index);
                m_size -= (end_index - start_index);
            }
            return begin() + start_index;
        }

        constexpr void swap(FixedVector& other) noexcept {
            if (this != &other) {
                std::swap_ranges(m_data.begin(), m_data.begin() + std::max(m_size, other.m_size), other.m_data.begin());
                std::swap(m_size, other.m_size);
            }
        }

        constexpr void clear() noexcept {
            m_size = 0;
        }


        //===============================================================
        // 比較演算子
        //===============================================================

        constexpr bool operator==(const FixedVector& other) const {
            return m_size == other.m_size && std::equal(begin(), end(), other.begin());
        }

        constexpr bool operator!=(const FixedVector& other) const {
            return !(*this == other);
        }

        constexpr bool operator<(const FixedVector& other) const {
            return std::lexicographical_compare(begin(), end(), other.begin(), other.end());
        }

        constexpr bool operator<=(const FixedVector& other) const {
            return !(other < *this);
        }

        constexpr bool operator>(const FixedVector& other) const {
            return other < *this;
        }

        constexpr bool operator>=(const FixedVector& other) const {
            return !(*this < other);
        }


    private:

        template<typename U>
        constexpr iterator insert_impl(const_iterator pos, U&& value) {
            if (m_size >= N) throw std::overflow_error("FixedVector: overflow");
            auto index = pos - cbegin();
            if (index < m_size) {
                std::move_backward(begin() + index, end(), end() + 1);
            }
            m_data[index] = std::forward<U>(value);
            ++m_size;
            return begin() + index;
        }

    private:
        std::array<T, N> m_data;
        size_type m_size = 0;
    };


}