//***********************************************************
//! @file
//! @brief		例外クラス
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Foundation/String/String.h>
#include "ExceptionType.h"

#undef GetMessage

namespace ob::foundation {

    //@―---------------------------------------------------------------------------
    //! @brief 例外クラス
    //@―---------------------------------------------------------------------------
    class Exception {
    public:
        using NotificationHandler = bool(*)(const Exception& e);		//!< 例外ハンドラ型
    public:

        //@―---------------------------------------------------------------------------
        //! @brief コンストラクタ
        //@―---------------------------------------------------------------------------
        Exception();


        //@―---------------------------------------------------------------------------
        //! @brief コンストラクタ(メッセージ指定)
        //@―---------------------------------------------------------------------------
        Exception(const String& message);


        //@―---------------------------------------------------------------------------
        //! @brief デストラクタ
        //@―---------------------------------------------------------------------------
        virtual ~Exception();


        //@―---------------------------------------------------------------------------
        //! @brief メッセージの取得
        //@―---------------------------------------------------------------------------
        virtual const String& message()const;


    protected:

        //@―---------------------------------------------------------------------------
        //! @brief			メッセージの設定
        //! 
        //! @param format	フォーマット文字列
        //! @param args		フォーマット引数
        //@―---------------------------------------------------------------------------
        void setMessage(const Char* format, va_list args);


    private:

        ExceptionLevel m_level;		//!< 例外レベル
        String m_message;			//!< メッセージ

    };

#pragma region Exceptions

    //! @cond
#define EXCEPTION_CONSTRUCTORS(typeName)	\
public:										\
	typeName():Exception(){}				\
	typeName(const String& message):Exception(message){}
//! @endcond

    //@―---------------------------------------------------------------------------
    //! @brief 無効な引数に対する例外クラス
    //@―---------------------------------------------------------------------------
    class ArgumentException :public Exception {
        //! @cond
        EXCEPTION_CONSTRUCTORS(ArgumentException)
            //! @endcond
    };

    //@―---------------------------------------------------------------------------
    //! @brief 算術演算の失敗に対する例外クラス
    //@―---------------------------------------------------------------------------
    class ArithmeticException :public Exception {
        //! @cond
        EXCEPTION_CONSTRUCTORS(ArithmeticException)
            //! @endcond
    };

    //@―---------------------------------------------------------------------------
    //! @brief 存在しないファイルパス/ディレクトリパスに対する例外クラス
    //@―---------------------------------------------------------------------------
    class PathNotFoundException :public Exception {
        //! @cond
        EXCEPTION_CONSTRUCTORS(PathNotFoundException)
            //! @endcond
    };

    //@―---------------------------------------------------------------------------
    //! @brief 値などの変換の失敗に対する例外クラス
    //@―---------------------------------------------------------------------------
    class FormatException :public Exception {
        //! @cond
        EXCEPTION_CONSTRUCTORS(FormatException)
            //! @endcond
    };

    //@―---------------------------------------------------------------------------
    //! @brief 範囲外アクセスに対する例外クラス
    //@―---------------------------------------------------------------------------
    class OutOfRangeException :public Exception {
        //! @cond
        EXCEPTION_CONSTRUCTORS(OutOfRangeException)
            //! @endcond
    };

    //@―---------------------------------------------------------------------------
    //! @brief 無効な状態でのメソッド呼び出しに対する例外クラス
    //@―---------------------------------------------------------------------------
    class InvalidOperationException :public Exception {
        //! @cond
        EXCEPTION_CONSTRUCTORS(InvalidOperationException)
            //! @endcond
    };

    //@―---------------------------------------------------------------------------
    //! @brief コレクションのアクセスに存在しないキーを使用した場合の例外クラス
    //@―---------------------------------------------------------------------------
    class KeyNotFoundException :public Exception {
        //! @cond
        EXCEPTION_CONSTRUCTORS(KeyNotFoundException)
            //! @endcond
    };

    //@―---------------------------------------------------------------------------
    //! @brief 未実装メソッド呼び出しに対する例外クラス
    //@―---------------------------------------------------------------------------
    class NotImplementedException :public Exception {
        //! @cond
        EXCEPTION_CONSTRUCTORS(NotImplementedException)
            //! @endcond
    };

    //@―---------------------------------------------------------------------------
    //! @brief サポートされていない操作に対する例外クラス
    //@―---------------------------------------------------------------------------
    class NotSupportedException :public Exception {
        //! @cond
        EXCEPTION_CONSTRUCTORS(NotSupportedException)
            //! @endcond
    };

#pragma endregion

}// namespace ob::foundation