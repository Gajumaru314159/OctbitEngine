//***********************************************************
//! @file
//! @brief		例外クラス
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Foundation/Base/Fwd.h>
#include <Foundation/Template/string/string.h>
#include "ExceptionType.h"

#undef GetMessage

namespace ob {

	//-----------------------------------------------------------------------------
	//! @brief 例外クラス
	//-----------------------------------------------------------------------------
	class Exception {
	public:
		using NotificationHandler = bool(*)(const Exception& e);
	public:
		Exception();
		Exception(const String& message);
		virtual ~Exception();

		virtual const String& GetMessage()const;

	protected:
		void SetMessage(const Char* format, va_list args);

	private:
		ExceptionLevel m_level;
		String m_message;
	};

#define EXCEPTION_CONSTRUCTORS(typeName)	\
public:										\
	typeName():Exception(){}				\
	typeName(const String& message):Exception(message){}

	//-----------------------------------------------------------------------------
	//! @brief 無効な引数に対する例外クラス
	//-----------------------------------------------------------------------------
	class ArgumentException :public Exception {
		EXCEPTION_CONSTRUCTORS(ArgumentException)
	};

	//-----------------------------------------------------------------------------
	//! @brief 算術演算の失敗に対する例外クラス
	//-----------------------------------------------------------------------------
	class ArithmeticException :public Exception {
		EXCEPTION_CONSTRUCTORS(ArithmeticException)
	};

	//-----------------------------------------------------------------------------
	//! @brief 存在しないファイルパス/ディレクトリパスに対する例外クラス
	//-----------------------------------------------------------------------------
	class PathNotFoundException :public Exception {
		EXCEPTION_CONSTRUCTORS(PathNotFoundException)
	};

	//-----------------------------------------------------------------------------
	//! @brief 値などの変換の失敗に対する例外クラス
	//-----------------------------------------------------------------------------
	class FormatException :public Exception {
		EXCEPTION_CONSTRUCTORS(FormatException)
	};

	//-----------------------------------------------------------------------------
	//! @brief 範囲外アクセスに対する例外クラス
	//-----------------------------------------------------------------------------
	class OutOfRangeException :public Exception {
		EXCEPTION_CONSTRUCTORS(OutOfRangeException)
	};

	//-----------------------------------------------------------------------------
	//! @brief 無効な状態でのメソッド呼び出しに対する例外クラス
	//-----------------------------------------------------------------------------
	class InvalidOperationException :public Exception {
		EXCEPTION_CONSTRUCTORS(InvalidOperationException)
	};

	//-----------------------------------------------------------------------------
	//! @brief コレクションのアクセスに存在しないキーを使用した場合の例外クラス
	//-----------------------------------------------------------------------------
	class KeyNotFoundException :public Exception {
		EXCEPTION_CONSTRUCTORS(KeyNotFoundException)
	};

	//-----------------------------------------------------------------------------
	//! @brief 未実装メソッド呼び出しに対する例外クラス
	//-----------------------------------------------------------------------------
	class NotImplementedException :public Exception {
		EXCEPTION_CONSTRUCTORS(NotImplementedException)
	};

	//-----------------------------------------------------------------------------
	//! @brief サポートされていない操作に対する例外クラス
	//-----------------------------------------------------------------------------
	class NotSupportedException :public Exception {
		EXCEPTION_CONSTRUCTORS(NotSupportedException)
	};

}// namespace ob