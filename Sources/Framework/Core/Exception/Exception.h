//***********************************************************
//! @file
//! @brief		例外クラス
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/core/String/String.h>
#include <Framework/Core/String/Format.h>
#include "ExceptionType.h"
#include <utility>

#undef GetMessage

namespace ob::core {

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
        Exception(StringView message);


        //@―---------------------------------------------------------------------------
        //! @brief デストラクタ
        //@―---------------------------------------------------------------------------
        virtual ~Exception();


        //@―---------------------------------------------------------------------------
        //! @brief メッセージを取得
        //@―---------------------------------------------------------------------------
        virtual const String& message()const;


    protected:

        //@―---------------------------------------------------------------------------
        //! @brief			メッセージを設定
        //! 
        //! @param format	フォーマット文字列
        //! @param args		フォーマット引数
        //@―---------------------------------------------------------------------------
        template <typename ...Args>
        void setMessage(StringView format, Args... args) {
            m_message = Format(format, std::forward(args)...);
        }


    private:

        ExceptionLevel m_level;		//!< 例外レベル
        String m_message;			//!< メッセージ

    };

#pragma region Exceptions

    //! @cond
#define EXCEPTION_CONSTRUCTORS(typeName)	\
public:										\
	typeName():Exception(){}				\
	typeName(StringView message):Exception(message){}
//! @endcond

    //@―---------------------------------------------------------------------------
    //! @brief 無効な引数に対する例外クラス
    //@―---------------------------------------------------------------------------
    class OB_API ArgumentException :public Exception {
        //! @cond
        EXCEPTION_CONSTRUCTORS(ArgumentException)
            //! @endcond
    };

    //@―---------------------------------------------------------------------------
    //! @brief 算術演算の失敗に対する例外クラス
    //@―---------------------------------------------------------------------------
    class OB_API ArithmeticException :public Exception {
        //! @cond
        EXCEPTION_CONSTRUCTORS(ArithmeticException)
            //! @endcond
    };

    //@―---------------------------------------------------------------------------
    //! @brief 存在しないファイルパス/ディレクトリパスに対する例外クラス
    //@―---------------------------------------------------------------------------
    class OB_API PathNotFoundException :public Exception {
        //! @cond
        EXCEPTION_CONSTRUCTORS(PathNotFoundException)
            //! @endcond
    };

    //@―---------------------------------------------------------------------------
    //! @brief 値などの変換の失敗に対する例外クラス
    //@―---------------------------------------------------------------------------
    class OB_API FormatException :public Exception {
        //! @cond
        EXCEPTION_CONSTRUCTORS(FormatException)
            //! @endcond
    };

    //@―---------------------------------------------------------------------------
    //! @brief 範囲外アクセスに対する例外クラス
    //@―---------------------------------------------------------------------------
    class OB_API OutOfRangeException :public Exception {
        //! @cond
        EXCEPTION_CONSTRUCTORS(OutOfRangeException)
            //! @endcond
    };

    //@―---------------------------------------------------------------------------
    //! @brief 無効な状態でのメソッド呼び出しに対する例外クラス
    //@―---------------------------------------------------------------------------
    class OB_API InvalidOperationException :public Exception {
        //! @cond
        EXCEPTION_CONSTRUCTORS(InvalidOperationException)
            //! @endcond
    };

    //@―---------------------------------------------------------------------------
    //! @brief コレクションのアクセスに存在しないキーを使用した場合の例外クラス
    //@―---------------------------------------------------------------------------
    class OB_API KeyNotFoundException :public Exception {
        //! @cond
        EXCEPTION_CONSTRUCTORS(KeyNotFoundException)
            //! @endcond
    };

    //@―---------------------------------------------------------------------------
    //! @brief 未実装メソッド呼び出しに対する例外クラス
    //@―---------------------------------------------------------------------------
    class OB_API NotImplementedException :public Exception {
        //! @cond
        EXCEPTION_CONSTRUCTORS(NotImplementedException)
            //! @endcond
    };

    //@―---------------------------------------------------------------------------
    //! @brief サポートされていない操作に対する例外クラス
    //@―---------------------------------------------------------------------------
    class OB_API NotSupportedException :public Exception {
        //! @cond
        EXCEPTION_CONSTRUCTORS(NotSupportedException)
            //! @endcond
    };

#pragma endregion

}// namespace ob::core