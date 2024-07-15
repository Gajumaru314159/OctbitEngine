//***********************************************************
//! @file
//! @brief		型情報
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Reflection/Type.h>
#include <Framework/Core/Reflection/DynamicCast.h>
#include <Framework/Core/Template/Utility/Memory.h>
#include <Framework/Core/Template/Utility/SequenceTraits.h>

namespace ob::core {

    class Any;

    class AnyReference {
    public:

        //! デフォルトコンストラクタ
        AnyReference() = default;

        //! 参照から構築
        template<class T, class = std::enable_if_t<!std::is_same_v<std::remove_const_t<T>, Any> && !std::is_same_v<std::remove_const_t<T>, AnyReference>>>
        AnyReference(T& value) { reset_impl(value); }

        //! 参照から代入
        template<class T, class = std::enable_if_t<!std::is_same_v<std::remove_const_t<T>, Any> && !std::is_same_v<std::remove_const_t<T>, AnyReference>>>
        AnyReference& operator=(T& value) { reset_impl(value); return *this; }

        //! 値を保持しているか
        bool empty()const { return m_pointer != nullptr; }

        //! 保持している値の型
        Type type() const { return m_type; }

        //! 指定した型にキャスト可能か
        bool isCastable(const Type& to) const { return IsCastable(type(), to); }
        template<typename ValueType>
        bool isCastable() const { return isCastable(Type::Get<ValueType>()); }

        //! 値を取得
        template<class T>
        std::remove_reference_t<T>& get() const {
            // TODO ダイナミックキャストチェックをできるようにする
            // if (!isCastable<T>()) throw std::bad_cast();
            return *reinterpret_cast<std::remove_reference_t<T>*>(m_pointer);
        }

    public:
#pragma region リストアクセス

        //! 異なる型のイテレータを共通操作するためのラッパー
        struct sequence_iterator_wrapper {
            virtual ~sequence_iterator_wrapper() = default;
            virtual AnyReference access() const = 0;
            virtual void increment() = 0;
            virtual bool equals(const sequence_iterator_wrapper& other)const = 0;
        };

        //! sequence_iterator_wrapperの特殊化
        template<class T>
        struct sequence_iterator_wrapper_template : sequence_iterator_wrapper {
            sequence_iterator_wrapper_template(typename T::iterator itr) : m_itr(itr) {}
            AnyReference access() const override { return *m_itr; };
            void increment() override { ++m_itr; }
            bool equals(const sequence_iterator_wrapper& other)const {
                return static_cast<const sequence_iterator_wrapper_template<T>*>(&other)->m_itr == m_itr;
            }
            typename T::iterator m_itr;
        };

        //! イテレータ本体
        class sequence_iterator {
        public:
            sequence_iterator(UPtr<sequence_iterator_wrapper> impl) : m_impl(std::move(impl)) {}
            AnyReference operator*() { return m_impl->access(); }
            sequence_iterator& operator++() { m_impl->increment(); return *this; }
            bool operator!=(const sequence_iterator& v) { return !m_impl->equals(*v.m_impl); }
        private:
            UPtr<sequence_iterator_wrapper> m_impl;
        };

        //! シーケンスアクセス
        class ListAccessor {
        public:
            using iterator_factory = Func<UPtr<sequence_iterator_wrapper>()>;
            using iterator_factory = Func<UPtr<sequence_iterator_wrapper>()>;
        public:
            ListAccessor() {}
            ListAccessor(iterator_factory begin, iterator_factory end) : m_begin(begin), m_end(end) {}
            sequence_iterator begin() const { return m_begin(); }
            sequence_iterator end() const { return m_end(); }
            operator bool() const { return m_begin || m_end; }
        private:
            iterator_factory m_begin;
            iterator_factory m_end;
        };

#pragma endregion

#pragma region マップアクセス

        //! 異なる型のイテレータを共通操作するためのラッパー
        struct map_iterator_wrapper {
            virtual ~map_iterator_wrapper() = default;
            virtual Pair<AnyReference, AnyReference> access() = 0;
            virtual void increment() = 0;
            virtual bool equals(const map_iterator_wrapper& other)const = 0;
        };

        //! イテレータ本体
        class map_iterator {
        public:
            map_iterator(UPtr<map_iterator_wrapper> impl) : m_impl(std::move(impl)) {}
            Pair<AnyReference, AnyReference> operator*() { return m_impl->access(); }
            map_iterator& operator++() { m_impl->increment(); return *this; }
            bool operator!=(const map_iterator& v) { return !m_impl->equals(*v.m_impl); }
        private:
            UPtr<map_iterator_wrapper> m_impl;
        };

        //! シーケンスアクセス
        class MapAccessor {
        public:
            using iterator_factory = Func<UPtr<map_iterator_wrapper>()>;
            using iterator_factory = Func<UPtr<map_iterator_wrapper>()>;
        public:
            MapAccessor() {}
            MapAccessor(iterator_factory begin, iterator_factory end) : m_begin(begin), m_end(end) {}
            map_iterator begin() const { return m_begin(); }
            map_iterator end() const { return m_end(); }
            operator bool() const { return m_begin || m_end; }
        private:
            iterator_factory m_begin;
            iterator_factory m_end;
        };

#pragma endregion

    public:

        const ListAccessor& list() const { return m_list; }
        const MapAccessor& map() const { return m_map; }

    private:

        template<class T>
        void reset_impl(T& value) {
            m_type = Type::Get<T>();
            m_pointer = const_cast<void*>(reinterpret_cast<const void*>(&value));
            m_list = {};
            m_map = {};
            if constexpr (is_sequence<T>::value) {
                m_list = ListAccessor(
                    [&] { return std::make_unique<sequence_iterator_wrapper_template<T>>(std::begin(value)); },
                    [&] { return std::make_unique<sequence_iterator_wrapper_template<T>>(std::end(value)); }
                    );
            }
            if constexpr (is_map<T>::value) {
                m_map = internal::CreateAnyReferenceMapAccessor(value);
            }
        }

    private:
        Type m_type;
        void* m_pointer = nullptr;
        ListAccessor m_list;
        MapAccessor m_map;
    };

    namespace internal {
        //! sequence_iterator_wrapperの特殊化
        template<class T>
        struct map_iterator_wrapper_template : AnyReference::map_iterator_wrapper {
            map_iterator_wrapper_template(typename T::iterator itr) : m_itr(itr) {}
            Pair<AnyReference, AnyReference> access() override { return { m_itr->first,m_itr->second }; };
            void increment() override { ++m_itr; }
            bool equals(const map_iterator_wrapper& other)const {
                return static_cast<const map_iterator_wrapper_template<T>*>(&other)->m_itr == m_itr;
            }
            typename T::iterator m_itr;
        };

        template<class T>
        AnyReference::MapAccessor CreateAnyReferenceMapAccessor(T& value) {
            return {
                [&] { return std::make_unique<map_iterator_wrapper_template<T>>(std::begin(value)); },
                [&] { return std::make_unique<map_iterator_wrapper_template<T>>(std::end(value)); }
            };
        }
    }
}