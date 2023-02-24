//***********************************************************
//! @file
//! @brief		デリゲート
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <functional>

namespace ob::core {

	//! @cond
	template<class SIGNATURE>
	class Delegate2 {};
	//! @endcond


	//@―---------------------------------------------------------------------------
	//! @brief      デリゲート
	//! 
	//! @details    インスタンスへの参照をメソッドへの参照をペアにしてカプセル化する。
	//@―---------------------------------------------------------------------------
	template<typename Ret, typename... Args>
	class Delegate2<Ret(Args...)> 
	{
	public:

		using this_type = typename ob::core::Delegate2<Ret(Args...)>;                            //!< this_type

		template<typename T>using method_type = Ret(T::*)(Args... args);                        //!< method_type
		template<typename T>using const_method_type = Ret(T::*)(Args... args)const;             //!< const_method_type

	public:

		//===============================================================
		// コンストラクタ / デストラクタ
		//===============================================================
		constexpr Delegate2()noexcept;                                   // デフォルトコンストラクタ
		Delegate2(const this_type& rhs)noexcept;                         // コピーコンストラクタ
		Delegate2(this_type&& rhs)noexcept;                              // ムーブコンストラクタ

		Delegate2(std::function<Ret(Args...)>&& func)noexcept {
			m_func = func;
		}

		template<class T>
		Delegate2(T* owner, method_type<T> func) {
			m_func = std::bind(func, owner);
		}

		template<class T>
		Delegate2(T* owner, const_method_type<T> func) {
			m_func = std::bind(func, owner);
		}

		Delegate2& operator = (std::function<Ret(Args...)>&& func)noexcept {
			m_func = func;
		}


		//===============================================================
		// オペレータ
		//===============================================================
		
		operator bool()const {
			return m_func;
		}

		Ret operator()(Args... args)const { return m_func(args...); }                              // 登録された関数を呼び出す

	private:

		std::function<Ret(Args..)> m_func;

	};

	

	//===============================================================
	// インライン関数
	//===============================================================


}// namespcae ob