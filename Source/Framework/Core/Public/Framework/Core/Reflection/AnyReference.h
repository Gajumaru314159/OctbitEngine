//***********************************************************
//! @file
//! @brief		型情報
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Reflection/Type.h>
#include <Framework/Core/Template/Utility/Memory.h>
#include <Framework/Core/Template/Utility/SequenceTraits.h>

namespace ob::core {
	
	class Any;

	class ConstAnyReference {
	public:
		class const_iterator_impl_interface {
		public:
			const_iterator_impl_interface() = default;
			virtual ~const_iterator_impl_interface() = default;
			virtual ConstAnyReference ref() { return {}; }
			virtual void increment() {}
			virtual bool equals(const const_iterator_impl_interface* other)const { return true; }
		};

		class const_iterator {
		public:
			const_iterator() { m_impl = std::make_unique<const_iterator_impl_interface>(); }
			const_iterator(UPtr<const_iterator_impl_interface> impl) { m_impl = std::move(impl); }
			ConstAnyReference operator*() { return m_impl->ref(); }
			const_iterator& operator++() { m_impl->increment(); return *this; }
			bool operator!=(const const_iterator& v) { return !m_impl->equals(v.m_impl.get()); }
		private:
			UPtr<const_iterator_impl_interface> m_impl;
		};

	public:
		ConstAnyReference() {}

		template<typename ValueType,class = std::enable_if_t<!std::is_same<ValueType,Any>::value>>
		ConstAnyReference(const ValueType& value) : m_holder(std::make_shared<ReadOnlyHolder<ValueType>>(value)) {}

		template<typename ValueType, class = std::enable_if_t<!std::is_same<ValueType, Any>::value>>
		ConstAnyReference& operator=(const ValueType& value) {
			m_holder = std::make_shared<ReadOnlyHolder<ValueType>>(value);
			return *this;
		}

		bool empty() const {
			return !m_holder;
		}

		Type type() const {
			return m_holder ? m_holder->type() : Type();
		}

		void reset() {
			m_holder.reset();
		}

		template<typename ValueType>
		const ValueType& get() const {
			if (Type::Get<ValueType>() != type()) throw std::bad_cast();
			return static_cast<ReadOnlyHolder<ValueType>*>(m_holder.get())->value;
		}



		bool isSequence()const {
			return m_holder ? m_holder->isSequence() : false;
		}

		const_iterator begin()const {
			return m_holder ? m_holder->begin() : const_iterator();
		}

		const_iterator end()const {
			return m_holder ? m_holder->end() : const_iterator();
		}

	protected:

		class HolderBase {
		public:
			virtual ~HolderBase() {}
			virtual Type type() const = 0;

			virtual bool isSequence()const = 0;
			virtual const_iterator begin()const = 0;
			virtual const_iterator end()const = 0;
		};

		template<typename ValueType>
		class ReadOnlyHolder : public HolderBase {
		public:
			class const_iterator_impl : public const_iterator_impl_interface {
			public:
				using const_iterator = typename ValueType::const_iterator;
			public:
				const_iterator_impl(const_iterator itr) : m_itr(itr) { }
				ConstAnyReference ref() override { return *m_itr; }
				void increment() override { ++m_itr; }
				bool equals(const const_iterator_impl_interface* other)const override {
					return static_cast<const const_iterator_impl*>(other)->m_itr == m_itr;
				}
			private:
				const_iterator m_itr;
			};
		public:
			ReadOnlyHolder(const ValueType& value) : value(value) {}
			Type type() const override { return Type::Get<ValueType>(); }

			bool isSequence()const {
				return is_sequence<ValueType>::value;
			}

			const_iterator begin()const {
				if constexpr (is_sequence<ValueType>::value)
					return std::make_unique<const_iterator_impl>(value.begin());
				else
					return {};
			}

			const_iterator end()const {
				if constexpr (is_sequence<ValueType>::value)
					return std::make_unique<const_iterator_impl>(value.end());
				else
					return {};
			}

			const ValueType& value;
		};

		template<typename ValueType>
		class Holder : public ReadOnlyHolder<ValueType> {
		public:
			Holder(ValueType& value) : ReadOnlyHolder(value), value(value) {}
			ValueType& value;
		};
	protected:
		ConstAnyReference(SPtr<HolderBase>&& holder) : m_holder(holder) {}
	protected:
		SPtr<HolderBase> m_holder;
	};





	class AnyReference : public ConstAnyReference {
	public:
		AnyReference() {}

		template<typename ValueType,class = std::enable_if_t<!std::is_const<std::remove_reference_t<ValueType>>::value>>
		AnyReference(ValueType& value) {
			m_holder = std::make_shared<Holder<ValueType>>(value);
		}

		template<typename ValueType, class = std::enable_if_t<!std::is_const<std::remove_reference_t<ValueType>>::value>>
		AnyReference& operator=(ValueType& value) {
			m_holder = std::make_shared<Holder<ValueType>>(value);
			return *this;
		}

		template<typename ValueType>
		ValueType& get() const {
			if (Type::Get<ValueType>() != type()) throw std::bad_cast();
			return static_cast<Holder<ValueType>*>(const_cast<HolderBase*>(m_holder.get()))->value;
		}
	};

}