//***********************************************************
//! @file
//! @brief		ログ出力
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Singleton/Singleton.h>
#include <Framework/Core/Template/include.h>
#include <Framework/Core/String/Format.h>
#include <Framework/Core/Thread/Mutex.h>
#include <Framework/Core/Thread/Atomic.h>
#include <Framework/Core/Log/LogTypes.h>


namespace ob::core {

    //@―---------------------------------------------------------------------------
    //! @brief      ロガー
    //! 
    //! @details    登録したイベントにログの追加を通知する。@n
    //!             このクラスではログの登録と通知のみを行うため、ログウィンドウへの
    //!             出力やログファイルへの保存をする場合は別途実装し登録する必要があ
    //!             る。
    //@―---------------------------------------------------------------------------
    class Logger :public Singleton<Logger> {
    public:

        using EventNotifier = EventNotifier<const Log&>;       //!< イベント・通知型
        using EventHandle = EventNotifier::Handle;              //!< イベント・ハンドル型
        using EventDelegateType = EventNotifier::delegate_type; //!< イベント・デリゲート型

        //@―---------------------------------------------------------------------------
        //! @brief メッセージの最大バイト数
        //@―---------------------------------------------------------------------------
        enum {
            MESSAGE_MAX = 2048, //! メッセージの最大バイト数
        };


    public:

        //@―---------------------------------------------------------------------------
        //! @brief コンストラクタ
        //@―---------------------------------------------------------------------------
        Logger();


        //@―---------------------------------------------------------------------------
        //! @brief デストラクタ
        //@―---------------------------------------------------------------------------
        ~Logger();


        //@―---------------------------------------------------------------------------
        //! @brief                  ログの追加
        //! 
        //! @details                この関数の呼び出しは LOG_INFO_EX や LOG_WARNING_EX マクロから呼び出される。@n
        //!                         直接呼び出しは非推奨です。
        //! @param type             ログの種類
        //! @param sourceLocation   ログ生成場所
        //! @param category         カテゴリ名
        //! @param pMessage         メッセージ
        //@―---------------------------------------------------------------------------
        void addLog(LogLevel level, const SourceLocation& sourceLocation, const Char* category, const Char* pMessage);  // ログの追加


        //@―---------------------------------------------------------------------------
        //! @brief                  ログの追加
        //! 
        //! @details                この関数の呼び出しは LOG_INFO_EX や LOG_WARNING_EX マクロから呼び出される。@n
        //!                         直接呼び出しは非推奨です。
        //! @param type             ログの種類
        //! @param sourceLocation   ログ生成場所
        //! @param category         カテゴリ名
        //! @param pFormat          フォーマット文字列
        //! @param ...args          フォーマット引数
        //@―---------------------------------------------------------------------------
        template<typename... Args>
        void addLog(LogLevel level, const SourceLocation& sourceLocation, const Char* category, const Char* pFormat, Args... args) {
            const String message = fmt::format(StringView(pFormat), ob::forward<Args>(args)...);
            addLog(level, sourceLocation, category, message.c_str());
        }


        //@―---------------------------------------------------------------------------
        //! @brief ログ・イベントの追加
        //@―---------------------------------------------------------------------------
        void addEvent(EventHandle& handle, EventDelegateType delegate);


        //@―---------------------------------------------------------------------------
        //! @brief ログ・イベントの削除
        //@―---------------------------------------------------------------------------
        void removeEvent(EventHandle& handle);


    private:

        Mutex           m_mutex;
        EventNotifier   m_notifier;
        std::atomic<bool>            m_logged = false;

    };


}// namespace ob::core