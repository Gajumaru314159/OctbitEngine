//***********************************************************
//! @file
//! @brief		イベント通知
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/core/Template/Delegate/Delegate.h>
#include <Framework/core/Template/Container/HandleList.h>
#include <Framework/Core/Thread/Mutex.h>
#include <Framework/Core/Thread/ScopeLock.h>

namespace ob::core {

    //@―---------------------------------------------------------------------------
    //! @brief              イベント通知クラス
    //! 
    //! @details            登録したイベントをまとめて呼び出すことができる。
    //! @tparam ...Args     イベント引数
    //@―---------------------------------------------------------------------------
    template<typename... Args>
    class EventNotifier :Noncopyable {
    public:

        using this_type = EventNotifier <Args...>;                                  //!< 型

        using delegate_type = ob::Delegate<void(Args...)>;                          //!< デリゲート型
        using function_type = void(Args...);                                        //!< 関数型
        template<typename T>using method_type = void(T::*)(Args...);                //!< メンバ関数ポインタ型
        template<typename T>using const_method_type = void(T::*)(Args...)const;     //!< constメンバ関数ポインタ型

        using Handle = typename HandleList<delegate_type>::Handle;                  //!< イベントハンドル型

    private:

        using handle_list_type = HandleList<delegate_type>;

    public:

        //===============================================================
        // コンストラクタ / デストラクタ
        //===============================================================
        EventNotifier();
        virtual ~EventNotifier();


        //===============================================================
        // 公開関数
        //===============================================================
        void add(Handle& handle, const delegate_type& delegate);                        // デリゲートをイベントとして登録する
        void add(Handle& handle, function_type& function);                              // 関数をイベントとして登録する
        template<class T>
        void add(Handle& handle, T& instance, method_type<T> pMethod);                  // メンバ関数をイベントとして登録する
        template<class T>
        void add(Handle& handle, const T& instance, const_method_type<T> pMethod);      // constメンバ関数をイベントとして登録する

        void clear()noexcept;                                                           // 登録したイベントをすべて削除する
        void remove(Handle& handle);                                                    // イベントを削除する

        void invoke(Args...)const;                                                      // イベントを呼び出す
        bool empty()const noexcept;                                                     // イベントが登録さえていないか判定する
        size_t size()const noexcept;                                                    // 登録されているイベントの数を取得する


    private:

        mutable SpinLock m_mutex;
        handle_list_type m_handleList;

    };






    //===============================================================
    // インライン関数
    //===============================================================

    //@―---------------------------------------------------------------------------
    //! @brief  コンストラクタ
    //@―---------------------------------------------------------------------------
    template<typename... Args>
    EventNotifier<Args...>::EventNotifier() {
        clear();
    }


    //@―---------------------------------------------------------------------------
    //! @brief  デストラクタ
    //@―---------------------------------------------------------------------------
    template<typename... Args>
    EventNotifier<Args...>::~EventNotifier() {
        clear();
    }


    //@―---------------------------------------------------------------------------
    //! @brief  デリゲートをイベントとして登録する
    //@―---------------------------------------------------------------------------
    template<typename... Args>
    void EventNotifier<Args...>::add(Handle& handle, const delegate_type& delegate) {
        handle.remove();
        {
            ScopeLock lock(m_mutex);
            m_handleList.push_back(handle, delegate);
        }
    }


    //@―---------------------------------------------------------------------------
    //! @brief  関数をイベントとして登録する
    //@―---------------------------------------------------------------------------
    template<typename... Args>
    void EventNotifier<Args...>::add(Handle& handle, function_type& function) {
        handle.remove();
        {
            ScopeLock lock(m_mutex);
            delegate_type delegate(function);
            m_handleList.emplace_back(handle, function);
        }
    }


    //@―---------------------------------------------------------------------------
    //! @brief  メンバ関数をイベントとして登録する
    //@―---------------------------------------------------------------------------
    template<typename... Args>
    template<class T>
    void EventNotifier<Args...>::add(Handle& handle, T& instance, method_type<T> pMethod) {
        handle.remove();
        {
            ScopeLock lock(m_mutex);
            delegate_type delegate(instance, pMethod);
            m_handleList.push_back(handle, delegate);
        }
    }


    //@―---------------------------------------------------------------------------
    //! @brief  constメンバ関数をイベントとして登録する
    //@―---------------------------------------------------------------------------
    template<typename... Args>
    template<class T>
    void EventNotifier<Args...>::add(Handle& handle, const T& instance, const_method_type<T> pMethod) {
        handle.remove();
        {
            ScopeLock lock(m_mutex);
            delegate_type delegate(instance, pMethod);
            m_handleList.push_back(handle, delegate);
        }
    }


    //@―---------------------------------------------------------------------------
    //! @brief  登録したイベントをすべて削除する
    //@―---------------------------------------------------------------------------
    template<typename... Args>
    void EventNotifier<Args...>::clear()noexcept {
        ScopeLock lock(m_mutex);
        m_handleList.clear();
    }


    //@―---------------------------------------------------------------------------
    //! @brief  イベントを削除する
    //@―---------------------------------------------------------------------------
    template<typename... Args>
    void EventNotifier<Args...>::remove(Handle& h) {
        ScopeLock lock(m_mutex);
        m_handleList.remove(h);
    }


    //@―---------------------------------------------------------------------------
    //! @brief  イベントを呼び出す
    //! 
    //! @param ...args 呼び出し引数
    //@―---------------------------------------------------------------------------
    template<typename... Args>
    void EventNotifier<Args...>::invoke(Args... args)const {
        ScopeLock lock(m_mutex);
        for (auto& e : m_handleList) {
            if (e.empty())continue;
            e(args...);
        }
    }


    //@―---------------------------------------------------------------------------
    //! @brief  イベントが登録さえていないか判定する
    //@―---------------------------------------------------------------------------
    template<typename... Args>
    bool EventNotifier<Args...>::empty()const noexcept {
        ScopeLock lock(m_mutex);
        return m_handleList.empty();
    }


    //@―---------------------------------------------------------------------------
    //! @brief  登録されているイベントの数を取得する
    //@―---------------------------------------------------------------------------
    template<typename... Args>
    size_t EventNotifier<Args...>::size()const noexcept {
        ScopeLock lock(m_mutex);
        return m_handleList.size();
    }

}// namespcae ob