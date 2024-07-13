//***********************************************************
//! @file
//! @brief		型情報
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Reflection/Type.h>
#include <Framework/Core/Template/Utility/Memory.h>

namespace ob::core {

	class AnyReference {
	public:
		AnyReference() {}

		template<typename ValueType>
		AnyReference(ValueType& value) : m_holder(new Holder<ValueType>(value)) {}

		template<typename ValueType>
		AnyReference(const ValueType& value) : m_holder(new ReadOnlyHolder<ValueType>(value)) {}

		template<typename ValueType>
		AnyReference& operator=(ValueType& value) {
			m_holder = new Holder<ValueType>(value);
			return *this;
		}

		template<typename ValueType>
		AnyReference& operator=(const ValueType& value) {
			m_holder = new ReadOnlyHolder<ValueType>(value);
			return *this;
		}

		bool empty() const {
			return !m_holder;
		}

		bool isConst() const {
			if (empty()) throw std::exception();
			return m_holder->isConst();
		}

		Type type() const {
			return m_holder ? m_holder->type() : Type();
		}

		void reset() {
			m_holder.reset();
		}

		template<typename ValueType>
		ValueType& get() {
			if (Type::Get<ValueType>() != type()) throw std::bad_cast();
			if (m_holder->isConst()) throw std::exception();
			return static_cast<AnyReference::Holder<ValueType>*>(m_holder.get())->value;
		}

		template<typename ValueType>
		const ValueType& get() const {
			if (Type::Get<ValueType>() != type()) throw std::bad_cast();
			return static_cast<AnyReference::ReadOnlyHolder<ValueType>*>(m_holder.get())->value;
		}

	private:
		class HolderBase {
		public:
			virtual ~HolderBase() {}
			virtual Type type() const = 0;
			virtual bool isConst() const = 0;
		};

		template<typename ValueType>
		class ReadOnlyHolder : public HolderBase {
		public:
			ReadOnlyHolder(const ValueType& value) : value(value) {}
			Type type() const override { return Type::Get<ValueType>(); }
			bool isConst() const override { return true; }
			const ValueType& value;
		};

		template<typename ValueType>
		class Holder : public ReadOnlyHolder<ValueType> {
		public:
			Holder(ValueType& value) : ReadOnlyHolder(value), value(value) {}
			bool isConst() const override { return false; }
			ValueType& value;
		};

	private:
		SPtr<HolderBase> m_holder;
	};



}