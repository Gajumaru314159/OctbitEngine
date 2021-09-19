//***********************************************************
//! @file
//! @brief		イベント通知
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Foundation/Base/Fwd.h>
#include <Foundation/Template/delegate/delegate.h>
#include <Foundation/Template/container/handle_list.h>
#include <Foundation/Template/thread/mutex.h>

namespace ob
{

    //-----------------------------------------------------------------------------
    //! @brief              イベント通知クラス
    //! 
    //! @details            登録したイベントをまとめて呼び出すことができる。
    //! @tparam ...Args     イベント引数
    //-----------------------------------------------------------------------------
    template<typename... Args>
    class event_notifier:private Noncopyable
    {
    public:

        using this_type = event_notifier <Args...>;                                 //!< 型

        using delegate_type = delegate<void(Args...)>;                              //!< デリゲート型
        using function_type = void(Args...);                                        //!< 関数型
        template<typename T>using method_type = void(T::*)(Args...);                //!< メンバ関数ポインタ型
        template<typename T>using const_method_type = void(T::*)(Args...)const;     //!< constメンバ関数ポインタ型

        using handle = typename handle_list<delegate_type>::handle;                 //!< イベントハンドル型

    private:

        using mutex_type = mutex;
        using handle_list_type = handle_list<delegate_type>;

    public:

        //===============================================================
        // コンストラクタ / デストラクタ
        //===============================================================
        event_notifier();
        virtual ~event_notifier();
        

        //===============================================================
        // 公開関数
        //===============================================================
        void add(handle& handle, const delegate_type& delegate);                        // デリゲートをイベントとして登録する
        void add(handle& handle, function_type& function);                              // 関数をイベントとして登録する
        template<class T>
        void add(handle& handle, T& instance, method_type<T> pMethod);                  // メンバ関数をイベントとして登録する
        template<class T>
        void add(handle& handle, const T& instance, const_method_type<T> pMethod);      // constメンバ関数をイベントとして登録する

        void clear()noexcept;                                                           // 登録したイベントをすべて削除する
        void remove(handle& handle);                                                    // イベントを削除する

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

    //-----------------------------------------------------------------------------
    //! @brief  コンストラクタ
    //-----------------------------------------------------------------------------
    template<typename... Args>
    event_notifier<Args...>::event_notifier()
    {
        clear();
    }


    //-----------------------------------------------------------------------------
    //! @brief  デストラクタ
    //-----------------------------------------------------------------------------
    template<typename... Args>
    event_notifier<Args...>::~event_notifier()
    {
        clear();
    }


    //-----------------------------------------------------------------------------
    //! @brief  デリゲートをイベントとして登録する
    //-----------------------------------------------------------------------------
    template<typename... Args>
    void event_notifier<Args...>::add(handle& handle, const delegate_type& delegate)
    {
        handle.remove();
        {
            lock_guard<mutex> lockGuard(m_mutex);
            m_handleList.push_back(handle,delegate);
        }
    }


    //-----------------------------------------------------------------------------
    //! @brief  関数をイベントとして登録する
    //-----------------------------------------------------------------------------
    template<typename... Args>
    void event_notifier<Args...>::add(handle& handle, function_type& function)
    {
        handle.remove();
        {
            lock_guard<mutex> lockGuard(m_mutex);
            delegate_type delegate(function);
            m_handleList.emplace_back(handle, function);
        }
    }


    //-----------------------------------------------------------------------------
    //! @brief  メンバ関数をイベントとして登録する
    //-----------------------------------------------------------------------------
    template<typename... Args>
    template<class T>
    void event_notifier<Args...>::add(handle& handle, T& instance, method_type<T> pMethod)
    {
        handle.remove();
        {
            lock_guard<mutex> lockGuard(m_mutex);
            delegate_type delegate(instance, pMethod);
            m_handleList.push_back(handle, delegate);
        }
    }


    //-----------------------------------------------------------------------------
    //! @brief  constメンバ関数をイベントとして登録する
    //-----------------------------------------------------------------------------
    template<typename... Args>
    template<class T>
    void event_notifier<Args...>::add(handle& handle,const T& instance, const_method_type<T> pMethod)
    {
        handle.remove();
        {
            lock_guard<mutex> lockGuard(m_mutex);
            delegate_type delegate(instance, pMethod);
            m_handleList.push_back(handle, delegate);
        }
    }


    //-----------------------------------------------------------------------------
    //! @brief  登録したイベントをすべて削除する
    //-----------------------------------------------------------------------------
    template<typename... Args>
    void event_notifier<Args...>::clear()noexcept     {
        lock_guard<mutex> lockGuard(m_mutex);
        m_handleList.clear();
    }


    //-----------------------------------------------------------------------------
    //! @brief  イベントを削除する
    //-----------------------------------------------------------------------------
    template<typename... Args>
    void event_notifier<Args...>::remove(handle& h)
    {
        lock_guard<mutex> lockGuard(m_mutex); 
        m_handleList.remove(h);
    }


    //-----------------------------------------------------------------------------
    //! @brief  イベントを呼び出す
    //! 
    //! @param ...args 呼び出し引数
    //-----------------------------------------------------------------------------
    template<typename... Args>
    void event_notifier<Args...>::invoke(Args... args)const
    {
        lock_guard<mutex> lockGuard(m_mutex);
        for (auto& event : m_handleList)
        {
            if (event.empty())continue;
            event(args...);
        }
    }


    //-----------------------------------------------------------------------------
    //! @brief  イベントが登録さえていないか判定する
    //-----------------------------------------------------------------------------
    template<typename... Args>
    bool event_notifier<Args...>::empty()const noexcept {
        lock_guard<mutex> lockGuard(m_mutex);
        return m_handleList.empty();
    }


    //-----------------------------------------------------------------------------
    //! @brief  登録されているイベントの数を取得する
    //-----------------------------------------------------------------------------
    template<typename... Args>
    size_t event_notifier<Args...>::size()const noexcept{
        lock_guard<mutex> lockGuard(m_mutex);
        return m_handleList.size();
    }


    //===============================================================
    // インライン関数(handle)
    //===============================================================
}// namespcae ob