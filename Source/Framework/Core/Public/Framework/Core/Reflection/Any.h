//***********************************************************
//! @file
//! @brief		型情報
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Reflection/Type.h>
#include <Framework/Core/Template/Utility/Memory.h>

namespace ob::core {

	class Any {
	private:
		struct PlaceHolder {
			virtual ~PlaceHolder() { }
			virtual Type const& type() const { return {}; }
			virtual UPtr<PlaceHolder> clone() const { return nullptr; }
		};

		template <typename T>
		struct Holder : public PlaceHolder {
		public:
			Holder(T const& it) : m_data(it) { }
			template <typename ...Args>
			Holder(Args ...args) : m_data(args) { }
			Type type() const override { return Type::Get<T>(); }
			UPtr<PlaceHolder> clone() const override { return std::make_unique<Holder<T>>(m_data); }
		public:
			T m_data;
		};
	public:
		Any() {}
		template <typename T>
		Any(T const& it) : m_data(new Holder<T>(it)) { }
		Any(Any const& it) : m_data(std::make_unique<PlaceHolder>(*it.m_data)) { }
		Any(Any&& it) : m_data(std::move(it.m_data)) { }
		Any& operator=(Any const& it) {
			m_data = it.m_data->clone();
			return *this;
		}
		Any& operator=(Any&& it) {
			m_data = std::move(it.m_data);
			return *this;
		}
		template <typename T>
		Any& operator=(T const& it) {
			m_data = std::make_unique<Holder<T>>(it);
			return *this;
		}

		template <typename T>
		T& get() const {
			if (m_data == nullptr || m_data->type() != Type::Get<T>()) {
				throw std::bad_cast();
			}

			auto p = reinterpret_cast<Holder<T>*>(m_data.get());
			return p->m_data;
		}

		Type type() const {
			return m_data->type();
		}
	private:
		UPtr<PlaceHolder> m_data;
	};


}