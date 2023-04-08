//***********************************************************
//! @file
//! @brief		スコープ・ユーティリティ
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Template/Utility/Function.h>

namespace ob::core {

	//@―---------------------------------------------------------------------------
	//! @brief		スコープ値
	//! 
	//! @details	スコープを抜けるときに値を元の状態、または指定した値に復元します。
	//!				```
	//!				s32 value = 0;
	//!				{
	//!					ScopedValue sv(value);
	//!					value = 1;
	//!					std::cout<<value<<std::endl;// 1
	//!				}
	//!				std::cout<<value<<std::endl;// 0
	//!				{
	//!					// 復元する値を2に指定
	//!					ScopedValue sv(value,2);
	//!					std::cout<<value<<std::endl;// 0
	//!				}
	//!				std::cout<<value<<std::endl;// 2
	//!				{
	//!					// 復元する値を4に指定して3で初期化
	//!					ScopedValue sv(value,3,4);
	//!					std::cout<<value<<std::endl;// 3
	//!				}
	//!				std::cout<<value<<std::endl;// 4
	//!				```
	//@―---------------------------------------------------------------------------
	template<typename T, typename V = T>
	class ScopeValue {
	public:

		//@―---------------------------------------------------------------------------
		//!	@brief					コンストラクタ
		//! 
		//! @details				スコープを抜けると value に restoreValue が代入されます。
		//! @param value			操作する変数
		//! @param initialValue		初期値
		//! @param restoreValue		復元する値
		//@―---------------------------------------------------------------------------
		ScopeValue(T& value, const V& initialValue, const V& restoreValue)
			: m_value(value)
			, m_restoreValue(restoreValue)
		{
			m_value = initialValue;
		}


		//@―---------------------------------------------------------------------------
		//!	@brief					コンストラクタ
		//! 
		//! @details				スコープを抜けると value に restoreValue が代入されます。
		//! @param value			操作する変数
		//! @param initialValue		初期値
		//@―---------------------------------------------------------------------------
		ScopeValue(T& value, const V& initialValue)
			: ScopeValue(value,initialValue,value)
		{

		}

		//@―---------------------------------------------------------------------------
		//!	@brief					コンストラクタ
		//! 
		//! @details				スコープを抜けると value に 構築時の値が代入されます。
		//! @param value			操作する変数
		//@―---------------------------------------------------------------------------
		ScopeValue(T& value)
			: m_value(value)
			, m_restoreValue(value)
		{
		}

		//@―---------------------------------------------------------------------------
		//!	@brief					デストラクタ
		//@―---------------------------------------------------------------------------
		~ScopeValue()
		{
			m_value = m_restoreValue;
		}

	private:
		T& m_value;
		V  m_restoreValue;
	};


	//@―---------------------------------------------------------------------------
	//! @brief		スコープ・アウト・アクション
	//! 
	//! @details	スコープを抜けるときに登録した処理を実行します。
	//!				エラーハンドリングなどに使用します。
	//!				ローカル変数をキャプチャする場合は解放順に注意が必要です。
	//@―---------------------------------------------------------------------------
	class ScopeOutAction
	{
	public:

		//@―---------------------------------------------------------------------------
		//!	@brief					コンストラクタ
		//@―---------------------------------------------------------------------------
		template<typename TProc>
		ScopeOutAction(const TProc& proc)
			: m_proc(proc)
		{}

		//@―---------------------------------------------------------------------------
		//!	@brief					コンストラクタ
		//@―---------------------------------------------------------------------------
		template<typename TProc>
		ScopeOutAction(TProc&& proc)
			: m_proc(std::forward<TProc>(proc))
		{}

		//@―---------------------------------------------------------------------------
		//!	@brief					デストラクタ
		//@―---------------------------------------------------------------------------
		~ScopeOutAction()
		{
			if (m_proc) {
				m_proc();
			}
		}

	private:
		Func<void()> m_proc;
	};

}// namespcae ob::core