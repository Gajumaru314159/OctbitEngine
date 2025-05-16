//***********************************************************
//! @file
//! @brief		
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Reflection/Type.h>
#include <Framework/Core/Utility/Noncopyable.h>
#include <Framework/Core/Utility/Nonmovable.h>

namespace ob::core {

	//! @brief 型ごとにインスタンスを格納・取得できるコンテナクラスです。型Tごとのインスタンスの参照取得、存在判定などの機能を提供します。コピー・ムーブは不可です。
	class AnyContainer : Nonmovable, Noncopyable {
	private:
		struct HolderBase { virtual ~HolderBase() = default; };
		template<class T> struct Holder : HolderBase { T instance; };
	public:

		AnyContainer() = default;

		//! @brief 指定した型Tのインスタンスへの参照を取得します。
		//! @tparam T 取得するインスタンスの型。
		//! @return 型Tのインスタンスへの参照。
		template<class T>
		auto get() -> std::enable_if_t<std::is_default_constructible<T>::value, T&> {
			constexpr Type type = Type::Get<T>();
			auto& holder = m_container[type];
			if (!holder) {
				holder = std::make_unique<Holder<T>>();
			}
			return reinterpret_cast<Holder<T>*>(holder.get())->instance;
		}

		//! @brief 指定した型のインスタンスへの定数参照を取得します。
		//! @tparam T 取得するインスタンスの型。
		//! @return 型Tのインスタンスへのconst参照。指定した型がコンテナに存在しない場合はアサートに失敗します。
		template<class T>
		auto get() const -> std::enable_if_t<std::is_default_constructible<T>::value, const T&> {
			constexpr Type type = Type::Get<T>();
			OB_ASSERT(m_container.contains(type), "{}はコンテナに含まれていません", type.name());
			auto& holder = m_container.at(type);
			return reinterpret_cast<const Holder<T>*>(holder.get())->instance;
		}

		//! @brief 指定した型の要素がコンテナに含まれているかどうかを判定します。
		//! @tparam T 検索する要素の型。
		//! @return 指定した型の要素がコンテナに含まれていれば true、そうでなければ false を返します。
		template<class T>
		bool contains() const {
			constexpr Type type = Type::Get<T>();
			return m_container.contains(type);
		}

		//! @brief 指定された型がコンテナに含まれているかどうかを判定します。
		//! @param type 検索対象となる型の参照。
		//! @return 型がコンテナに含まれていれば true、そうでなければ false を返します。
		bool contains(const Type& type) const {
			return m_container.contains(type);
		}

	private:
		HashMap<Type, UPtr<HolderBase>> m_container;
	};

}