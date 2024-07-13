//***********************************************************
//! @file
//! @brief		型情報
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Reflection/Type.h>
#include <Framework/Core/Template/Utility/Memory.h>

namespace ob::core {

	class ConstAnyReference {
	public:
		ConstAnyReference() {}

		template<typename ValueType>
		ConstAnyReference(const ValueType& value) : m_holder(std::make_shared<ReadOnlyHolder<ValueType>>(value)) {}

		template<typename ValueType>
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

	protected:

		class HolderBase {
		public:
			virtual ~HolderBase() {}
			virtual Type type() const = 0;
		};

		template<typename ValueType>
		class ReadOnlyHolder : public HolderBase {
		public:
			ReadOnlyHolder(const ValueType& value) : value(value) {}
			Type type() const override { return Type::Get<ValueType>(); }
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