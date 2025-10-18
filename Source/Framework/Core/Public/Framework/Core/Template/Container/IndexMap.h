//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <cstddef>
#include <iterator>
#include <initializer_list>
#include <type_traits>
#include <stdexcept>
#include <Framework/Core/Log/Assertion.h>
#include <Framework/Core/Template/Allocator/STLAllocator.h>
#include <Framework/Core/Template/Container/Vector.h>
#include <Framework/Core/Template/Container/Map.h>
#include <Framework/Core/Template/Utility/Pair.h>
#include <Framework/Core/Template/Utility/Optional.h>

namespace ob::core {

	//! @brief 安定した反復順序を持つ順序付きハッシュマップ
	//! @details VectorストレージとHashMapインデックスにより挿入順序を維持しながら、平均O(1)の検索/更新を実現します。
	//!          内部のキーとインデックスのマッピングが壊れないようにするため、キーは挿入後も不変である必要があります。
	template<
		class TKey,
		class TValue,
		class THasher = std::hash<TKey>,
		class TKeyEqual = std::equal_to<TKey>,
		class TAllocator = ob::core::STLAllocator<Pair<TKey, TValue>>
	>
	class IndexMap {
	private:
		using key_allocator_type = typename std::allocator_traits<TAllocator>::template rebind_alloc<TKey>;
		using mapped_allocator_type = typename std::allocator_traits<TAllocator>::template rebind_alloc<TValue>;
		using key_storage_type = Vector<TKey, key_allocator_type>;
		using mapped_storage_type = Vector<TValue, mapped_allocator_type>;
		using index_value_type = Pair<const TKey, std::size_t>;
		using index_allocator_type = typename std::allocator_traits<TAllocator>::template rebind_alloc<index_value_type>;
		using index_map_type = HashMap<TKey, std::size_t, THasher, TKeyEqual, index_allocator_type>;

	public:
		using key_type = TKey;
		using mapped_type = TValue;
		using value_type = Pair<TKey, TValue>;
		using size_type = std::size_t;
		using difference_type = std::ptrdiff_t;
		using hasher = THasher;
		using key_equal = TKeyEqual;
		using allocator_type = TAllocator;

	private:
		struct value_reference {
			key_type& first;
			mapped_type& second;

			value_reference(key_type& key, mapped_type& value) : first(key), second(value) {}
			value_reference(const value_reference&) = default;
			value_reference& operator=(const value_reference& other) {
				first = other.first;
				second = other.second;
				return *this;
			}
			value_reference& operator=(const value_type& other) {
				first = other.first;
				second = other.second;
				return *this;
			}
			operator value_type() const {
				return value_type(first, second);
			}
		};

		struct const_value_reference {
			const key_type& first;
			const mapped_type& second;

			const_value_reference(const key_type& key, const mapped_type& value) : first(key), second(value) {}
			const_value_reference(const value_reference& ref) : first(ref.first), second(ref.second) {}
			operator value_type() const {
				return value_type(first, second);
			}
		};

		template<bool IsConst>
		struct pointer_proxy;

		template<>
		struct pointer_proxy<false> {
			value_type value;
			explicit pointer_proxy(value_reference ref) : value(ref) {}
			value_type* operator->() { return &value; }
			const value_type* operator->() const { return &value; }
		};

		template<>
		struct pointer_proxy<true> {
			value_type value;
			explicit pointer_proxy(const_value_reference ref) : value(ref) {}
			const value_type* operator->() const { return &value; }
		};

		template<bool IsConst>
		class iterator_base {
			using owner_type = std::conditional_t<IsConst, const IndexMap*, IndexMap*>;
			owner_type m_owner = nullptr;
			size_type m_index = 0;
			template<bool>
			friend class iterator_base;
			friend class IndexMap;
		public:
			using iterator_category = std::random_access_iterator_tag;
			using value_type = IndexMap::value_type;
			using difference_type = IndexMap::difference_type;
			using reference = std::conditional_t<IsConst, const_value_reference, value_reference>;
			using pointer = pointer_proxy<IsConst>;

			iterator_base() = default;
			iterator_base(owner_type owner, size_type index) : m_owner(owner), m_index(index) {}
			template<bool B = IsConst, std::enable_if_t<B, int> = 0>
			iterator_base(const iterator_base<false>& other) : m_owner(other.m_owner), m_index(other.m_index) {}

			reference operator*() const {
				return reference(m_owner->m_keys[m_index], m_owner->m_values[m_index]);
			}

			pointer operator->() const {
				return pointer(**this);
			}

			reference operator[](difference_type n) const {
				return *(*this + n);
			}

			iterator_base& operator++() {
				++m_index;
				return *this;
			}

			iterator_base operator++(int) {
				iterator_base tmp = *this;
				++(*this);
				return tmp;
			}

			iterator_base& operator--() {
				--m_index;
				return *this;
			}

			iterator_base operator--(int) {
				iterator_base tmp = *this;
				--(*this);
				return tmp;
			}

			iterator_base& operator+=(difference_type n) {
				m_index = static_cast<size_type>(static_cast<difference_type>(m_index) + n);
				return *this;
			}

			iterator_base& operator-=(difference_type n) {
				m_index = static_cast<size_type>(static_cast<difference_type>(m_index) - n);
				return *this;
			}

			iterator_base operator+(difference_type n) const {
				iterator_base tmp = *this;
				return tmp += n;
			}

			iterator_base operator-(difference_type n) const {
				iterator_base tmp = *this;
				return tmp -= n;
			}

			difference_type operator-(const iterator_base& other) const {
				return static_cast<difference_type>(m_index) - static_cast<difference_type>(other.m_index);
			}

			bool operator==(const iterator_base& other) const {
				return m_owner == other.m_owner && m_index == other.m_index;
			}

			bool operator!=(const iterator_base& other) const {
				return !(*this == other);
			}

			bool operator<(const iterator_base& other) const {
				return m_index < other.m_index;
			}

			bool operator>(const iterator_base& other) const {
				return other < *this;
			}

			bool operator<=(const iterator_base& other) const {
				return !(other < *this);
			}

			bool operator>=(const iterator_base& other) const {
				return !(*this < other);
			}
		};

	public:
		using reference = value_reference;
		using const_reference = const_value_reference;
		using pointer = value_type*;
		using const_pointer = const value_type*;
		using iterator = iterator_base<false>;
		using const_iterator = iterator_base<true>;
		using reverse_iterator = std::reverse_iterator<iterator>;
		using const_reverse_iterator = std::reverse_iterator<const_iterator>;

	public:
		static constexpr size_type npos = static_cast<size_type>(-1);

	public:
		IndexMap() = default;

		explicit IndexMap(size_type capacity) {
			reserve(capacity);
		}

		IndexMap(std::initializer_list<value_type> init) {
			reserve(init.size());
			insert(init);
		}

		IndexMap(const IndexMap&) = default;
		IndexMap(IndexMap&&) noexcept = default;
		IndexMap& operator=(const IndexMap&) = default;
		IndexMap& operator=(IndexMap&&) noexcept = default;

		IndexMap& operator=(std::initializer_list<value_type> init) {
			clear();
			reserve(init.size());
			insert(init);
			return *this;
		}

	public:
		[[nodiscard]] size_type size() const noexcept {
			return m_values.size();
		}

		[[nodiscard]] bool empty() const noexcept {
			return m_values.empty();
		}

		void reserve(size_type newCapacity) {
			m_keys.reserve(newCapacity);
			m_values.reserve(newCapacity);
			m_index.reserve(newCapacity);
		}

		[[nodiscard]] size_type capacity() const noexcept {
			return m_values.capacity();
		}

		mapped_type* data() noexcept {
			return m_values.data();
		}

		const mapped_type* data() const noexcept {
			return m_values.data();
		}

		void shrink_to_fit() {
			m_keys.shrink_to_fit();
			m_values.shrink_to_fit();

			index_map_type rebuilt;
			rebuilt.reserve(m_values.size());
			for (size_type i = 0; i < m_values.size(); ++i) {
				rebuilt.emplace(m_keys[i], i);
			}
			m_index.swap(rebuilt);
		}

		void clear() noexcept {
			m_keys.clear();
			m_values.clear();
			m_index.clear();
		}

		iterator begin() noexcept { return iterator(this, 0); }
		const_iterator begin() const noexcept { return const_iterator(this, 0); }
		const_iterator cbegin() const noexcept { return const_iterator(this, 0); }

		iterator end() noexcept { return iterator(this, size()); }
		const_iterator end() const noexcept { return const_iterator(this, size()); }
		const_iterator cend() const noexcept { return const_iterator(this, size()); }

		reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }
		const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(end()); }
		const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(cend()); }

		reverse_iterator rend() noexcept { return reverse_iterator(begin()); }
		const_reverse_iterator rend() const noexcept { return const_reverse_iterator(begin()); }
		const_reverse_iterator crend() const noexcept { return const_reverse_iterator(cbegin()); }

	public:
		iterator find(const key_type& key) noexcept {
			auto it = m_index.find(key);
			return it == m_index.end() ? end() : iterator(this, it->second);
		}

		const_iterator find(const key_type& key) const noexcept {
			auto it = m_index.find(key);
			return it == m_index.end() ? cend() : const_iterator(this, it->second);
		}

		[[nodiscard]] bool contains(const key_type& key) const noexcept {
			return m_index.find(key) != m_index.end();
		}

		[[nodiscard]] size_type index_of(const key_type& key) const noexcept {
			auto it = m_index.find(key);
			return it == m_index.end() ? npos : it->second;
		}

		reference at_index(size_type index) {
			OB_ASSERT_EXPR(index < size());
			return reference(m_keys[index], m_values[index]);
		}

		const_reference at_index(size_type index) const {
			OB_ASSERT_EXPR(index < size());
			return const_reference(m_keys[index], m_values[index]);
		}

		mapped_type* try_at_index(size_type index) noexcept {
			return index < size() ? &m_values[index] : nullptr;
		}

		const mapped_type* try_at_index(size_type index) const noexcept {
			return index < size() ? &m_values[index] : nullptr;
		}

		mapped_type& at(const key_type& key) {
			auto it = m_index.find(key);
			if (it == m_index.end()) {
				throw std::out_of_range("IndexMap::at: key not found");
			}
			return m_values[it->second];
		}

		const mapped_type& at(const key_type& key) const {
			auto it = m_index.find(key);
			if (it == m_index.end()) {
				throw std::out_of_range("IndexMap::at: key not found");
			}
			return m_values[it->second];
		}

		mapped_type* try_get(const key_type& key) noexcept {
			auto it = m_index.find(key);
			return it == m_index.end() ? nullptr : &m_values[it->second];
		}

		const mapped_type* try_get(const key_type& key) const noexcept {
			auto it = m_index.find(key);
			return it == m_index.end() ? nullptr : &m_values[it->second];
		}

		mapped_type& operator[](const key_type& key) {
			return try_emplace_impl(key).first.operator*().second;
		}

		template<class K>
		mapped_type& operator[](K&& key) {
			return try_emplace_impl(std::forward<K>(key)).first.operator*().second;
		}

	public:
		Pair<iterator, bool> insert(const value_type& value) {
			auto found = m_index.find(value.first);
			if (found != m_index.end()) {
				return { iterator(this, found->second), false };
			}

			m_keys.push_back(value.first);
			try {
				m_values.push_back(value.second);
			}
			catch (...) {
				m_keys.pop_back();
				throw;
			}

			auto index = size() - 1;
			m_index.emplace(m_keys[index], index);
			return { iterator(this, index), true };
		}

		Pair<iterator, bool> insert(value_type&& value) {
			auto found = m_index.find(value.first);
			if (found != m_index.end()) {
				return { iterator(this, found->second), false };
			}

			m_keys.push_back(std::move(value.first));
			try {
				m_values.push_back(std::move(value.second));
			}
			catch (...) {
				m_keys.pop_back();
				throw;
			}

			auto index = size() - 1;
			m_index.emplace(m_keys[index], index);
			return { iterator(this, index), true };
		}

		iterator insert([[maybe_unused]] const_iterator hint, const value_type& value) {
			return insert(value).first;
		}

		iterator insert([[maybe_unused]] const_iterator hint, value_type&& value) {
			return insert(std::move(value)).first;
		}

		template<class InputIt>
		void insert(InputIt first, InputIt last) {
			for (; first != last; ++first) {
				insert(*first);
			}
		}

		void insert(std::initializer_list<value_type> init) {
			insert(init.begin(), init.end());
		}

		template<class... Args>
		Pair<iterator, bool> emplace(Args&&... args) {
			value_type value(std::forward<Args>(args)...);
			return insert(std::move(value));
		}

		template<class... Args>
		Pair<iterator, bool> try_emplace(const key_type& key, Args&&... args) {
			return try_emplace_impl(key, std::forward<Args>(args)...);
		}

		template<class K, class... Args>
		Pair<iterator, bool> try_emplace(K&& key, Args&&... args) {
			return try_emplace_impl(std::forward<K>(key), std::forward<Args>(args)...);
		}

		template<class M>
		Pair<iterator, bool> insert_or_assign(const key_type& key, M&& obj) {
			auto it = m_index.find(key);
			if (it != m_index.end()) {
				m_values[it->second] = std::forward<M>(obj);
				return { iterator(this, it->second), false };
			}

			m_keys.push_back(key);
			try {
				m_values.emplace_back(std::forward<M>(obj));
			}
			catch (...) {
				m_keys.pop_back();
				throw;
			}

			auto index = size() - 1;
			m_index.emplace(m_keys[index], index);
			return { iterator(this, index), true };
		}

		template<class K, class M>
		Pair<iterator, bool> insert_or_assign(K&& key, M&& obj) {
			auto it = m_index.find(key);
			if (it != m_index.end()) {
				m_values[it->second] = std::forward<M>(obj);
				return { iterator(this, it->second), false };
			}

			m_keys.emplace_back(std::forward<K>(key));
			try {
				m_values.emplace_back(std::forward<M>(obj));
			}
			catch (...) {
				m_keys.pop_back();
				throw;
			}

			auto index = size() - 1;
			m_index.emplace(m_keys[index], index);
			return { iterator(this, index), true };
		}

		reference front() {
			OB_ASSERT_EXPR(!empty());
			return reference(m_keys.front(), m_values.front());
		}

		const_reference front() const {
			OB_ASSERT_EXPR(!empty());
			return const_reference(m_keys.front(), m_values.front());
		}

		reference back() {
			OB_ASSERT_EXPR(!empty());
			return reference(m_keys.back(), m_values.back());
		}

		const_reference back() const {
			OB_ASSERT_EXPR(!empty());
			return const_reference(m_keys.back(), m_values.back());
		}

	public:
		Optional<value_type> pop_back() {
			if (empty()) {
				return Optional<value_type>();
			}
			return Optional<value_type>(remove_swap_last(size() - 1));
		}

		bool erase(const key_type& key) {
			return static_cast<bool>(erase_entry(key));
		}

		Optional<value_type> erase_entry(const key_type& key) {
			auto it = m_index.find(key);
			if (it == m_index.end()) {
				return Optional<value_type>();
			}
			return Optional<value_type>(remove_preserve_order(it->second));
		}

		Optional<value_type> erase_index(size_type index) {
			if (index >= size()) {
				return Optional<value_type>();
			}
			return Optional<value_type>(remove_preserve_order(index));
		}

		Optional<value_type> swap_erase_index(size_type index) {
			if (index >= size()) {
				return Optional<value_type>();
			}
			return Optional<value_type>(remove_swap_last(index));
		}

		Optional<value_type> swap_erase(const key_type& key) {
			auto it = m_index.find(key);
			if (it == m_index.end()) {
				return Optional<value_type>();
			}
			return Optional<value_type>(remove_swap_last(it->second));
		}

		iterator erase(const_iterator pos) {
			auto index = pos.m_index;
			erase_index(index);
			return iterator(this, index);
		}

		iterator erase(const_iterator first, const_iterator last) {
			if (first == last) {
				return iterator(this, first.m_index);
			}

			size_type start = first.m_index;
			size_type count = static_cast<size_type>(last.m_index - first.m_index);
			for (size_type i = 0; i < count; ++i) {
				m_index.erase(m_keys[start + i]);
			}

			m_keys.erase(m_keys.begin() + static_cast<difference_type>(start),
				m_keys.begin() + static_cast<difference_type>(start + count));
			m_values.erase(m_values.begin() + static_cast<difference_type>(start),
				m_values.begin() + static_cast<difference_type>(start + count));

			rebuild_index_from(start);
			return iterator(this, start);
		}

	public:
		void swap(IndexMap& other) noexcept {
			using std::swap;
			swap(m_keys, other.m_keys);
			swap(m_values, other.m_values);
			swap(m_index, other.m_index);
		}

		friend void swap(IndexMap& lhs, IndexMap& rhs) noexcept {
			lhs.swap(rhs);
		}

	private:
		template<class K, class... Args>
		Pair<iterator, bool> try_emplace_impl(K&& key, Args&&... args) {
			auto found = m_index.find(key);
			if (found != m_index.end()) {
				return { iterator(this, found->second), false };
			}

			m_keys.emplace_back(std::forward<K>(key));
			try {
				m_values.emplace_back(std::forward<Args>(args)...);
			}
			catch (...) {
				m_keys.pop_back();
				throw;
			}

			auto index = size() - 1;
			m_index.emplace(m_keys[index], index);
			return { iterator(this, index), true };
		}

		void rebuild_index_from(size_type start) {
			for (size_type i = start; i < size(); ++i) {
				m_index[m_keys[i]] = i;
			}
		}

		value_type remove_preserve_order(size_type index) {
			OB_ASSERT_EXPR(index < size());
			value_type removed(m_keys[index], m_values[index]);
			key_type keyCopy = m_keys[index];

			m_keys.erase(m_keys.begin() + static_cast<difference_type>(index));
			m_values.erase(m_values.begin() + static_cast<difference_type>(index));
			m_index.erase(keyCopy);
			rebuild_index_from(index);

			return removed;
		}

		value_type remove_swap_last(size_type index) {
			OB_ASSERT_EXPR(index < size());
			value_type removed(m_keys[index], m_values[index]);
			const size_type lastIndex = size() - 1;
			key_type keyCopy = m_keys[index];

			m_index.erase(keyCopy);

			if (index != lastIndex) {
				m_keys[index] = std::move(m_keys[lastIndex]);
				m_values[index] = std::move(m_values[lastIndex]);
				m_index[m_keys[index]] = index;
			}

			m_keys.pop_back();
			m_values.pop_back();

			return removed;
		}

	private:
		key_storage_type m_keys;
		mapped_storage_type m_values;
		index_map_type m_index;
	};

}
