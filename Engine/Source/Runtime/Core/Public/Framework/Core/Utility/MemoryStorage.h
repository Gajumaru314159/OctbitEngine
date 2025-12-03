//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <type_traits>
#include <Framework/Core/Utility/Noncopyable.h>
#include <Framework/Core/Utility/Nonmovable.h>
#include <Framework/Core/Log/Assertion.h>

namespace ob::core {

	//! @brief		メモリストレージ
	//! 
	//! @details	遅延初期化をする場合に使用します。
	//!				通常はRAIIに則り生成時に初期化することが望ましいですが、
	//!				初期化を遅らせる必要がある場合に使用します。
	//!				unique_ptrと違いメンバ変数として使用する場合はクラスのインクルードが必要です。
	template<class T,size_t SIZE,size_t ALIGN>
	class MemoryStorageBase : Noncopyable,Nonmovable{
	public:

		//! @brief  コンストラクタ
		MemoryStorageBase()
			: m_constructed(false)
		{
		}

		//! @brief  デストラクタ
		~MemoryStorageBase() {
			if (m_constructed)destruct();
		}

		//! @brief  構築済みか
		operator bool()const noexcept {
			return m_constructed;
		}

		//! @brief  空か
		bool empty()const noexcept {
			return !m_constructed;
		}

		//! @brief  構築
		template<typename ...Args>
		auto construct(Args&&... args) -> std::enable_if_t<std::is_constructible_v<T,Args...>,void> {
			static_assert(sizeof(T) <= SIZE);
			if (!m_constructed) {
				new(m_data) T(std::forward<Args>(args)...);
				m_constructed = true;
			}
		}

		//! @brief  破壊
		void destruct() {
			if (m_constructed) {
				reinterpret_cast<T*>(m_data)->~T();
				m_constructed = false;
			}
		}

		//! @brief  ポインタアクセス
		T* operator ->() {
			OB_ASSERT(!empty(), "空のMemoryStorageにアクセスしました。");
			return reinterpret_cast<T*>(m_data);
		}

		//! @brief  ポインタアクセス(const)
		const T* operator ->()const {
			OB_ASSERT(!empty(), "空のMemoryStorageにアクセスしました。");
			return reinterpret_cast<const T*>(m_data);
		}

		//! @brief  参照アクセス
		T& operator *() {
			OB_ASSERT(!empty(), "空のMemoryStorageにアクセスしました。");
			return *reinterpret_cast<T*>(m_data);
		}

		//! @brief  参照アクセス(const)
		const T& operator *()const noexcept {
			OB_ASSERT(!empty(), "空のMemoryStorageにアクセスしました。");
			return *reinterpret_cast<const T*>(m_data);
		}

	private:

		alignas(ALIGN) std::byte m_data[SIZE] {};
		bool m_constructed;

	};
		
	template<class T>
	using MemoryStorage = MemoryStorageBase<T, sizeof(T), alignof(T)>;

}