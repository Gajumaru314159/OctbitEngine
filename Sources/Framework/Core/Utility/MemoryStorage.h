﻿//***********************************************************
//! @file
//! @brief		メモリストレージ
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <type_traits>
#include <Framework/Core/Template/include.h>
#include <Framework/Core/Log/Assertion.h>

namespace ob::core {

	//@―---------------------------------------------------------------------------
	//! @brief		メモリストレージ
	//! 
	//! @details	遅延初期化をする場合に使用します。
	//!				通常はRAIIに則り生成時に初期化することが望ましいですが、
	//!				初期化を遅らせる必要がある場合に使用します。
	//!				uniqque_ptrと違いメンバ変数として使用する場合はクラスのインクルードが必要です。
	//@―---------------------------------------------------------------------------
	template<class T>
	class MemoryStorage {
	public:

		//@―---------------------------------------------------------------------------
		//! @brief  コンストラクタ
		//@―---------------------------------------------------------------------------
		MemoryStorage()
			: m_constructed(false)
		{
			static_assert(sizeof(m_data) == sizeof(T));
		}

		//@―---------------------------------------------------------------------------
		//! @brief  デストラクタ
		//@―---------------------------------------------------------------------------
		~MemoryStorage() {
			if (m_constructed)destruct();
		}

		//@―---------------------------------------------------------------------------
		//! @brief  構築済みか
		//@―---------------------------------------------------------------------------
		bool operator()const noexcept {
			return m_constructed;
		}

		//@―---------------------------------------------------------------------------
		//! @brief  空か
		//@―---------------------------------------------------------------------------
		bool empty()const noexcept {
			return !m_constructed;
		}

		//@―---------------------------------------------------------------------------
		//! @brief  構築
		//@―---------------------------------------------------------------------------
		template<typename ...Args>
		void construct(Args&&... args) {
			if (m_constructed) {
				destruct();
			}
		}

		//@―---------------------------------------------------------------------------
		//! @brief  破壊
		//@―---------------------------------------------------------------------------
		void destruct() {
			if (m_constructed) {
				reinterpret_cast<T*>(&m_data)->~T();
				m_constructed = false;
			}
		}

		//@―---------------------------------------------------------------------------
		//! @brief  ポインタアクセス
		//@―---------------------------------------------------------------------------
		T* operator ->() {
			OB_ASSERT(empty(), "空のMemoryStorageにアクセスしました。");
			return reinterpret_cast<T*>(&m_data);
		}

		//@―---------------------------------------------------------------------------
		//! @brief  ポインタアクセス(const)
		//@―---------------------------------------------------------------------------
		const T* operator ->()const {
			OB_ASSERT(empty(), "空のMemoryStorageにアクセスしました。");
			return reinterpret_cast<const T*>(&m_data);
		}

		//@―---------------------------------------------------------------------------
		//! @brief  参照アクセス
		//@―---------------------------------------------------------------------------
		T& operator *() const {
			OB_ASSERT(empty(), "空のMemoryStorageにアクセスしました。");
			return *reinterpret_cast<T*>(&m_data);
		}

		//@―---------------------------------------------------------------------------
		//! @brief  参照アクセス(const)
		//@―---------------------------------------------------------------------------
		const T& operator *()const noexcept {
			OB_ASSERT(empty(), "空のMemoryStorageにアクセスしました。");
			return *reinterpret_cast<const T*>(&m_data);
		}

	private:

		std::aligned_storage_t<sizeof(T), alignof(T)> m_data;
		bool m_constructed;

	};

}// namespcae ob