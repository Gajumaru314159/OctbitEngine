//***********************************************************
//! @file
//! @brief		アサーション
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Core/Base/BaseType.hpp>
#include <Core/Base/BaseDef.hpp>
#include <Core/Template/String.hpp>

//============================================
// 例外発生基底マクロ
//============================================

//! @brief 基底のアサーションマクロ
#define OB_ASSERTION_BASE(level,exception,format,...) ob::detail::NotifyException<exception>(level, TC(__FILE__), __LINE__, format, ##__VA_ARGS__)

//! @brief 基底のチェックマクロ
#define OB_CHECK_BASE(expr,level,exception) (!(expr)) && ob::detail::NotifyException<exception>(level, TC(__FILE__), __LINE__, TC(#expr))


//============================================
// 例外発生マクロ
//============================================

//! @brief      アプリケーション実行中に発生した軽微な問題を通知する
//! @details    アプリケーションの継続が可能である。
#define OB_WARNING(format,...)	OB_ASSERTION_BASE(ob::ExceptionLevel::Warning, ob::Exception, format, __VA_ARGS__)

//! @brief      アプリケーションが不正な状態になる可能性がある問題を通知する(Error)
//! @details    アプリケーションの継続が危険であり、早急に終了することが望まれる。
#define OB_ERROR(format,...)	OB_ASSERTION_BASE(ob::ExceptionLevel::Error, ob::Exception, format, __VA_ARGS__)

//! @brief      アプリケーションの継続が難しい致命的なエラーを通知する
//! @details    アプリケーションは直ちに終了しなければならない。
#define OB_FATAL(format,...)	OB_ASSERTION_BASE(ob::ExceptionLevel::Fatal, ob::Exception, format, __VA_ARGS__)


//! @brief      到達不能コードをマークするためのマクロ
//! @details    式が呼び出された場合、例外ハンドラが呼び出される。
#define OB_UNREACHABLE() OB_ASSERTION_BASE(ob::ExceptionLevel::Fatal, ob::LogicException, "Unreachable code.")

//! @brief      未実装の機能をマークする
//! @details    式が呼び出された場合、例外ハンドラが呼び出される。
#define OB_NOTIMPLEMENTED() OB_ASSERTION_BASE(ob::ExceptionLevel::Fatal, ob::NotImplementedException, "Not implemented code.")


//============================================
// 式評価による例外発生マクロ
//============================================

//! @brief      関数やロジックを実行する前の前提条件を確認する
//! @details    式がfalseである場合、例外ハンドラが呼び出される。
#define OB_REQUIRE(expr) OB_CHECK_BASE(expr,ob::ExceptionLevel::Warning, ob::Exception)

//! @brief      関数やロジックを実行する前の前提条件を確認する
//! @details    式がfalseである場合、例外ハンドラが呼び出される。
#define OB_ENSURE(expr) OB_CHECK_BASE(expr,ob::ExceptionLevel::Error, ob::Exception)

//! @brief      OutOfRangeの検出マクロ
//! @details    min<value<maxでない場合警告を発生させる
#define OB_REQUIRE_RANGE(value, min, max) OB_REQUIRE(min <= value && value < max)


//============================================
// エラーチェック
//============================================

namespace ob
{
    namespace detail
    {

        //! @brief 強制終了
        //! @param file    強制終了が呼び出されたファイル名
        //! @param line    強制終了が呼び出された行数
        //! @param message 通知メッセージ
        void NotifyAbort(const Char* file, int line, const Char* message);

    } // namespace detail
} // namespace ob



#ifdef OB_DEBUG
//! @brief デバッグチェックフラグ
#	define	OB_DEBUG_CHECK_ENABLED
#endif


#ifndef OB_ABORT
#	include <stdlib.h>
//! @brief NotifyAbortの呼び出し
#	define OB_ABORT ::ob::detail::NotifyAbort(TC(__FILE__), __LINE__, TC("abort."));
#endif


#ifndef OB_CHECK
//! @brief 式を評価して偽である場合異常終了させる
#	define OB_CHECK(expr) if (!OB_LIKELY(expr)) { printf("%s:%u: abort: %s\n", (__FILE__), (unsigned)(__LINE__), (#expr)); OB_ABORT; }
#endif


#ifdef OB_DEBUG_CHECK_ENABLED
#	define OB_DEBUG_CHECK(expr) OB_CHECK(expr);
#else
#	define OB_DEBUG_CHECK(expr) ((void)(expr));
#endif

//-----------------------------------------------------------
//! @def OB_DCHECK
//! @brief 式を評価して偽である場合異常終了させる(デバッグモード用)
//-----------------------------------------------------------



//============================================
// 例外
//============================================

namespace ob
{

    //-----------------------------------------------------------
    //! @brief  例外の通知レベル
    //-----------------------------------------------------------
    enum class ExceptionLevel
    {
        Fatal,		//!< 強制終了
        Error,		//!< エラー出力
        Warning,	//!< 警告表示
        Info,		//!< 情報表示
    };


    //-----------------------------------------------------------
    //! @brief  例外クラス
    //-----------------------------------------------------------
    class OB_API Exception
    {
    public:

        using NotificationHandler = bool(*)(Exception& e);      //!< 通知ハンドラの関数ポインタ型

    public:
        //-----------------------------------------------------------
        // コンストラクタ
        //-----------------------------------------------------------
        Exception();                        //!< デフォルトコンストラクタ
        Exception(const Char* message);     //!< コンストラクタ(例外メッセージ指定)

        //-----------------------------------------------------------
        //! @brief デストラクタ
        //-----------------------------------------------------------
        virtual ~Exception();

        //-----------------------------------------------------------
        //! @brief 例外の説明をを取得
        //-----------------------------------------------------------
        virtual inline const String& GetMessage() const { return m_message; };

        //-----------------------------------------------------------
        //! @brief                  例外の発生場所の設定
        //! 
        //! @param level            例外レベル
        //! @param filePath         例外の発生したファイル名
        //! @param fileLine         例外の発生した行数
        //! @param assertionMessage 例外メッセージ
        //-----------------------------------------------------------
        void SetSourceLocationInfo(ExceptionLevel level, const Char* filePath, int fileLine, const Char* assertionMessage);

        //-----------------------------------------------------------
        //! @brief          エラーハンドラの設定
        //! 
        //! @param handler  エラーハンドラ
        //-----------------------------------------------------------
        static void SetNotificationHandler(NotificationHandler handler);

        //-----------------------------------------------------------
        //! @brief エラーハンドラの取得
        //-----------------------------------------------------------
        static NotificationHandler GetNotificationHandler();

    protected:

        //-----------------------------------------------------------
        //! @brief キャプションの設定
        //-----------------------------------------------------------
        inline void SetCaption(const Char* caption) { m_caption = caption; }

        //-----------------------------------------------------------
        //! @brief キャプションの取得
        //-----------------------------------------------------------
        inline const String& GetCaption() { return m_caption; };

        //-----------------------------------------------------------
        //! @brief          例外メッセージの設定
        //! 
        //! @param message  例外メッセージ
        //-----------------------------------------------------------
        inline void SetMessage(const Char* message) { m_message = GetCaption() + message; }

        //-----------------------------------------------------------
        //! @brief          例外メッセージの設定
        //! 
        //! @param format   フォーマット
        //! @param args     可変長引数
        //-----------------------------------------------------------
        void SetMessage(const Char* format, va_list args);

    private:

        ExceptionLevel m_level;		//!< 例外レベル	
        String m_sourceFilePath;	//!< 例外の発生したソースファイル名
        s32 m_sourceFileLine;		//!< 例外の発生した行数
        String m_assertionMessage;	//!< 例外メッセージ

        String	m_caption;			//!< キャプション
        String	m_message;			//!< メッセージ

    };



    //-----------------------------------------------------------
    //! @brief 前提条件の間違いなどプログラム内の論理的な誤りが原因で発生したエラーを表す
    //-----------------------------------------------------------
    class OB_API LogicException
        : public Exception
    {
    public:
        //-----------------------------------------------------------
        // コンストラクタ
        //-----------------------------------------------------------
        LogicException();                        //!< デフォルトコンストラクタ
        LogicException(const Char* message);     //!< コンストラクタ(例外メッセージ指定)
    };



    //-----------------------------------------------------------
    //! @brief 主にアプリケーションの実行環境が原因で発生したエラーを表す
    //-----------------------------------------------------------
    class OB_API  RuntimeException
        : public Exception
    {
    public:
        //-----------------------------------------------------------
        // コンストラクタ
        //-----------------------------------------------------------
        RuntimeException();                      //!< デフォルトコンストラクタ
        RuntimeException(const Char* message);   //!< コンストラクタ(例外メッセージ指定)
    };



    //-----------------------------------------------------------
    //! @brief アプリケーションの継続が難しい致命的なエラーを表す
    //-----------------------------------------------------------
    class OB_API FatalException
        : public Exception
    {
    public:
        //-----------------------------------------------------------
        // コンストラクタ
        //-----------------------------------------------------------
        FatalException();                        //!< デフォルトコンストラクタ
        FatalException(const Char* message);     //!< コンストラクタ(例外メッセージ指定)
    };




    //------------------------------------------------------------------------------
    // extension errors
    //------------------------------------------------------------------------------



    //-----------------------------------------------------------
    //! @brief 未実装の機能を呼び出した場合のエラーを表す
    //-----------------------------------------------------------
    class OB_API NotImplementedException : public LogicException
    {
    public:
        //-----------------------------------------------------------
        // コンストラクタ
        //-----------------------------------------------------------
        NotImplementedException();                          //!< デフォルトコンストラクタ
        NotImplementedException(const Char* message);       //!< コンストラクタ(例外メッセージ指定)
    };



    //-----------------------------------------------------------
    //! @brief I/O エラーが発生した場合のエラーを表す
    //-----------------------------------------------------------
    class OB_API IOException : public RuntimeException
    {
    public:
        //-----------------------------------------------------------
        // コンストラクタ
        //-----------------------------------------------------------
        IOException();                                      //!< デフォルトコンストラクタ
        IOException(const Char* message);                   //!< コンストラクタ(例外メッセージ指定)
    };

    namespace detail
    {

        //-----------------------------------------------------------
        //! @brief 例外の通知
        //! @tparam TException 例外クラス
        //! @tparam ...TArgs ログに出力する変数群
        //! @param level 例外レベル
        //! @param file 例外発生ファイル
        //! @param line 例外発生行
        //! @param exprString 条件式の文字列表現
        //! @param ...args ログに出力する変数群
        //! @return 例外の出力に失敗した場合falseを返す
        //-----------------------------------------------------------
        template<class TException, typename... TArgs>
        inline bool NotifyException(ExceptionLevel level, const Char* file, int line, const Char* exprString, TArgs... args)
        {
            const size_t BUFFER_SIZE = 512;
            Char str[BUFFER_SIZE] = TC("AAAAAA");
            // TODO errorPrintf(str, BUFFER_SIZE, args...);

            TException e(str);
            e.SetSourceLocationInfo(level, file, line, exprString);

            auto h = Exception::GetNotificationHandler();
            if (h != nullptr && h(e))
            {
                return true;
            }

            //printError(e);

            if (level == ExceptionLevel::Fatal || level == ExceptionLevel::Error)
            {
                throw e;
            }

            return true;
        }
    } // namespace detail
} // namespace ob