#pragma once
#include <array>
#include <stdexcept>
#include <iterator>
#include <algorithm>
#include <initializer_list>
#include <utility>
#include <Framework/Core/Template/Utility/TypeTraits.h>

namespace ob::core {

    //@―---------------------------------------------------------------------------
    //! @brief  Fixed配列
    //@―---------------------------------------------------------------------------
    template<typename T, std::size_t N>
    class FixedVector {
    public:
        using value_type = T;
        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;
        using reference = value_type&;
        using const_reference = const value_type&;
        using pointer = value_type*;
        using const_pointer = const value_type*;
        using iterator = typename std::array<T, N>::iterator;
        using const_iterator = typename std::array<T, N>::const_iterator;
        using reverse_iterator = typename std::array<T, N>::reverse_iterator;
        using const_reverse_iterator = typename std::array<T, N>::const_reverse_iterator;
    public:

        FixedVector() : m_size(0) {}

        FixedVector(size_type size)
            : m_size(size) 
        {
            if (size > N) throw std::overflow_error("FixedVector: overflow");
        }

        FixedVector(size_type size, const T& value) 
            : m_size(size) 
        {
            if (size > N) throw std::overflow_error("FixedVector: overflow");
            std::fill(m_data.begin(), m_data.begin() + size, value);
        }

        FixedVector(std::initializer_list<T> init)
            : m_size(init.size())
        {
            if (init.size() > N) throw std::overflow_error("FixedVector: overflow");
            std::copy(init.begin(), init.end(), m_data.begin());
        }

        template<class InputIt,std::enable_if_t<is_iterator<InputIt>::value>>
        FixedVector(InputIt first, InputIt last) {
            size_type count = std::distance(first, last);
            if (count > N) throw std::overflow_error("FixedVector: overflow");
            std::copy(first, last, m_data.begin());
            m_size = count;
        }

        reference at(size_type pos) {
            if (pos >= m_size) throw std::out_of_range("FixedVector: out of range");
            return m_data[pos];
        }

        const_reference at(size_type pos) const {
            if (pos >= m_size) throw std::out_of_range("FixedVector: out of range");
            return m_data[pos];
        }

        reference operator[](size_type pos) {
            return m_data[pos];
        }

        const_reference operator[](size_type pos) const {
            return m_data[pos];
        }

        reference front() {
            return m_data[0];
        }

        const_reference front() const {
            return m_data[0];
        }

        reference back() {
            return m_data[m_size - 1];
        }

        const_reference back() const {
            return m_data[m_size - 1];
        }

        iterator begin() noexcept {
            return m_data.begin();
        }

        const_iterator begin() const noexcept {
            return m_data.begin();
        }

        const_iterator cbegin() const noexcept {
            return m_data.cbegin();
        }

        iterator end() noexcept {
            return m_data.begin() + m_size;
        }

        const_iterator end() const noexcept {
            return m_data.begin() + m_size;
        }

        const_iterator cend() const noexcept {
            return m_data.cbegin() + m_size;
        }

        reverse_iterator rbegin() noexcept {
            return m_data.rbegin();
        }

        const_reverse_iterator rbegin() const noexcept {
            return m_data.rbegin();
        }

        const_reverse_iterator crbegin() const noexcept {
            return m_data.crbegin();
        }

        reverse_iterator rend() noexcept {
            return m_data.rbegin() + m_size;
        }

        const_reverse_iterator rend() const noexcept {
            return m_data.rbegin() + m_size;
        }

        const_reverse_iterator crend() const noexcept {
            return m_data.crbegin() + m_size;
        }

        bool empty() const noexcept {
            return m_size == 0;
        }

        size_type size() const noexcept {
            return m_size;
        }

        size_type capacity() const noexcept {
            return N;
        }

        constexpr size_type max_size() const noexcept {
            return N;
        }

        void clear() noexcept {
            m_size = 0;
        }

        void push_back(const T& value) {
            if (m_size >= N) throw std::overflow_error("FixedVector: overflow");
            m_data[m_size++] = value;
        }

        void push_back(T&& value) {
            if (m_size >= N) throw std::overflow_error("FixedVector: overflow");
            m_data[m_size++] = std::move(value);
        }

        template<class... Args>
        reference emplace_back(Args&&... args) {
            if (m_size >= N) throw std::overflow_error("FixedVector: overflow");
            m_data[m_size] = T(std::forward<Args>(args)...);
            return m_data[m_size++];
        }

        void pop_back() {
            if (m_size > 0) --m_size;
        }

        void resize(size_type count) {
            if (count > N) throw std::overflow_error("FixedVector: overflow");
            m_size = count;
        }

        void resize(size_type count, const value_type& value) {
            if (count > N) throw std::overflow_error("FixedVector: overflow");
            if (count > m_size) {
                std::fill(m_data.begin() + m_size, m_data.begin() + count, value);
            }
            m_size = count;
        }

        void assign(size_type n, const T& t) {
            std::fill(m_data.begin(),m_data.begin()+n,t);
        }

        template<class InputIt, std::enable_if_t<is_iterator<InputIt>::value>>
        void assign(InputIt first, InputIt last) {
            size_type count = std::distance(first, last);
            if (count > N) throw std::overflow_error("FixedVector: overflow");
            std::copy(first, last, m_data.begin());
            m_size = count;
        }    // Single element insert
        iterator insert(const_iterator pos, const T& value) {
            return insert_impl(pos, value);
        }

        // Move element insert
        iterator insert(const_iterator pos, T&& value) {
            return insert_impl(pos, std::move(value));
        }

        // Fill insert
        iterator insert(const_iterator pos, size_type count, const T& value) {
            if (m_size + count > N) throw std::overflow_error("FixedVector: overflow");
            auto index = pos - cbegin();
            if (count > 0) {
                std::move_backward(begin() + index, end(), end() + count);
                std::fill(begin() + index, begin() + index + count, value);
                m_size += count;
            }
            return begin() + index;
        }

        // Range insert
        template<class InputIt, std::enable_if_t<is_iterator<InputIt>::value>>
        iterator insert(const_iterator pos, InputIt first, InputIt last) {
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

        // Initializer list insert
        iterator insert(const_iterator pos, std::initializer_list<T> ilist) {
            return insert(pos, ilist.begin(), ilist.end());
        }
        // Erase single element
        iterator erase(const_iterator pos) {
            auto index = pos - cbegin();
            if (index < m_size) {
                std::move(begin() + index + 1, end(), begin() + index);
                --m_size;
            }
            return begin() + index;
        }

        // Erase range of elements
        iterator erase(const_iterator first, const_iterator last) {
            auto start_index = first - cbegin();
            auto end_index = last - cbegin();
            if (start_index < end_index && end_index <= m_size) {
                std::move(begin() + end_index, end(), begin() + start_index);
                m_size -= (end_index - start_index);
            }
            return begin() + start_index;
        }
        // Swap function
        void swap(FixedVector& other) noexcept {
            if (this != &other) {
                std::swap_ranges(m_data.begin(), m_data.begin() + std::max(m_size, other.m_size), other.m_data.begin());
                std::swap(m_size, other.m_size);
            }
        }
        // Emplace function
        template<class... Args>
        iterator emplace(const_iterator pos, Args&&... args) {
            if (m_size >= N) throw std::overflow_error("FixedVector: overflow");
            auto index = pos - cbegin();
            if (index < m_size) {
                std::move_backward(begin() + index, end(), end() + 1);
            }
            m_data[index] = T(std::forward<Args>(args)...);
            ++m_size;
            return begin() + index;
        }
        // Equality comparison
        bool operator==(const FixedVector& other) const {
            return m_size == other.m_size && std::equal(begin(), end(), other.begin());
        }

        // Inequality comparison
        bool operator!=(const FixedVector& other) const {
            return !(*this == other);
        }
        // Less than comparison
        bool operator<(const FixedVector& other) const {
            return std::lexicographical_compare(begin(), end(), other.begin(), other.end());
        }

        // Less than or equal to comparison
        bool operator<=(const FixedVector& other) const {
            return !(other < *this);
        }

        // Greater than comparison
        bool operator>(const FixedVector& other) const {
            return other < *this;
        }

        // Greater than or equal to comparison
        bool operator>=(const FixedVector& other) const {
            return !(*this < other);
        }


    private:
        template<typename U>
        iterator insert_impl(const_iterator pos, U&& value) {
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
        size_type m_size;
    };


}