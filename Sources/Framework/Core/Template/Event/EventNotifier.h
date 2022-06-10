﻿//***********************************************************
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
    class event_notifier :Noncopyable {
    public:

        using this_type = event_notifier <Args...>;                                 //!< 型

        using delegate_type = delegate<void(Args...)>;                              //!< デリゲート型
        using function_type = void(Args...);                                        //!< 関数型
        template<typename T>using method_type = void(T::*)(Args...);                //!< メンバ関数ポインタ型
        template<typename T>using const_method_type = void(T::*)(Args...)const;     //!< constメンバ関数ポインタ型

        using Handle = typename HandleList<delegate_type>::Handle;                 //!< イベントハンドル型

    private:

        using mutex_type = Mutex;
        using lock_type = ScopeLock<mutex_type> ;
        using handle_list_type = HandleList<delegate_type>;

    public:

        //===============================================================
        // コンストラクタ / デストラクタ
        //===============================================================
        event_notifier();
        virtual ~event_notifier();


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

        mutable mutex_type m_mutex;
        handle_list_type m_handleList;

    };






    //===============================================================
    // インライン関数
    //===============================================================

    //@―---------------------------------------------------------------------------
    //! @brief  コンストラクタ
    //@―---------------------------------------------------------------------------
    template<typename... Args>
    event_notifier<Args...>::event_notifier() {
        clear();
    }


    //@―---------------------------------------------------------------------------
    //! @brief  デストラクタ
    //@―---------------------------------------------------------------------------
    template<typename... Args>
    event_notifier<Args...>::~event_notifier() {
        clear();
    }


    //@―---------------------------------------------------------------------------
    //! @brief  デリゲートをイベントとして登録する
    //@―---------------------------------------------------------------------------
    template<typename... Args>
    void event_notifier<Args...>::add(Handle& Handle, const delegate_type& delegate) {
        Handle.remove();
        {
            lock_type lockGuard(m_mutex);
            m_handleList.push_back(Handle, delegate);
        }
    }


    //@―---------------------------------------------------------------------------
    //! @brief  関数をイベントとして登録する
    //@―---------------------------------------------------------------------------
    template<typename... Args>
    void event_notifier<Args...>::add(Handle& handle, function_type& function) {
        handle.remove();
        {
            lock_type lockGuard(m_mutex);
            delegate_type delegate(function);
            m_handleList.emplace_back(handle, function);
        }
    }


    //@―---------------------------------------------------------------------------
    //! @brief  メンバ関数をイベントとして登録する
    //@―---------------------------------------------------------------------------
    template<typename... Args>
    template<class T>
    void event_notifier<Args...>::add(Handle& handle, T& instance, method_type<T> pMethod) {
        handle.remove();
        {
            lock_type lockGuard(m_mutex);
            delegate_type delegate(instance, pMethod);
            m_handleList.push_back(handle, delegate);
        }
    }


    //@―---------------------------------------------------------------------------
    //! @brief  constメンバ関数をイベントとして登録する
    //@―---------------------------------------------------------------------------
    template<typename... Args>
    template<class T>
    void event_notifier<Args...>::add(Handle& handle, const T& instance, const_method_type<T> pMethod) {
        handle.remove();
        {
            lock_type lockGuard(m_mutex);
            delegate_type delegate(instance, pMethod);
            m_handleList.push_back(handle, delegate);
        }
    }


    //@―---------------------------------------------------------------------------
    //! @brief  登録したイベントをすべて削除する
    //@―---------------------------------------------------------------------------
    template<typename... Args>
    void event_notifier<Args...>::clear()noexcept {
        lock_type lockGuard(m_mutex);
        m_handleList.clear();
    }


    //@―---------------------------------------------------------------------------
    //! @brief  イベントを削除する
    //@―---------------------------------------------------------------------------
    template<typename... Args>
    void event_notifier<Args...>::remove(Handle& h) {
        lock_type lockGuard(m_mutex);
        m_handleList.remove(h);
    }


    //@―---------------------------------------------------------------------------
    //! @brief  イベントを呼び出す
    //! 
    //! @param ...args 呼び出し引数
    //@―---------------------------------------------------------------------------
    template<typename... Args>
    void event_notifier<Args...>::invoke(Args... args)const {
        lock_type lockGuard(m_mutex);
        for (auto& e : m_handleList) {
            if (e.empty())continue;
            e(args...);
        }
    }


    //@―---------------------------------------------------------------------------
    //! @brief  イベントが登録さえていないか判定する
    //@―---------------------------------------------------------------------------
    template<typename... Args>
    bool event_notifier<Args...>::empty()const noexcept {
        lock_type lockGuard(m_mutex);
        return m_handleList.empty();
    }


    //@―---------------------------------------------------------------------------
    //! @brief  登録されているイベントの数を取得する
    //@―---------------------------------------------------------------------------
    template<typename... Args>
    size_t event_notifier<Args...>::size()const noexcept {
        lock_type lockGuard(m_mutex);
        return m_handleList.size();
    }

}// namespcae ob