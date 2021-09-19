//***********************************************************
//! @file
//! @brief		スピン・ミューテックス
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Foundation/Base/Fwd.h>
#include <Foundation/Template/atomic.h>
#include <Foundation/Log/LogMacro.h>
#include "exponential_backoff.h"


namespace ob
{

    //@―-------------------------------------------------------------------------------
    //! @brief スピン・ミューテックス
    //@―-------------------------------------------------------------------------------
    class spin_mutex
    {
    public:

        spin_mutex(bool isLocked = false);
        ~spin_mutex();
        OB_DISALLOW_COPY(spin_mutex);

        void lock();
        bool try_lock();
        void unlock();


    private:

        atomic<bool> m_syncFlag;

    };



    class recursive_spin_mutex
    {
    private:

        struct lock_data
        {
            thread::id threadID;       //!< スレッドID
            s32 lockCount;      //!< ロック回数
        };

    public:

        recursive_spin_mutex(bool isLocked = false);
        ~recursive_spin_mutex();
        OB_DISALLOW_COPY(recursive_spin_mutex);

        void lock();
        bool try_lock();
        void unlock();


    private:

        atomic<lock_data> m_syncFlag;

    };






    //===============================================================
    // インライン関数
    //===============================================================

    //@―-------------------------------------------------------------------------------
    inline spin_mutex::spin_mutex(bool isLocked)
    {
        m_syncFlag.store(isLocked,memory_order::memory_order_release);
    }


    inline spin_mutex::~spin_mutex()
    {

    }

    inline void spin_mutex::lock()
    {
        exponential_backoff backoff;
        while (true)
        {
            auto expected = false;
            if (m_syncFlag.compare_exchange_weak(expected, true, memory_order::memory_order_acq_rel, memory_order::memory_order_acquire))
            {
                break;
            }
            backoff.wait();
        }
    }

    inline bool spin_mutex::try_lock()
    {
        bool expected = false;
        return m_syncFlag.compare_exchange_weak(expected, true, memory_order::memory_order_acq_rel, memory_order::memory_order_acquire);
    }

    inline void spin_mutex::unlock()
    {
        m_syncFlag.store(false,memory_order::memory_order_release);
    }




    inline recursive_spin_mutex::recursive_spin_mutex(bool isLocked)
    {
        lock_data newValue;
        if (isLocked)
        {
            newValue.threadID   = this_thread::get_id();
            newValue.lockCount  = 1;
        }
        else
        {
            newValue.threadID   = thread::id();
            newValue.lockCount  = 0;
        }
        m_syncFlag.store(newValue, memory_order::memory_order_release);
    }


    inline recursive_spin_mutex::~recursive_spin_mutex()
    {

    }

    inline void recursive_spin_mutex::lock()
    {
        exponential_backoff backoff;
        auto threadID = this_thread::get_id();
        while (true)
        {
            auto nowValue = m_syncFlag.load(memory_order::memory_order_acquire);
            if ((nowValue.threadID == threadID) || (nowValue.threadID == thread::id()))
            {
                lock_data newValue;
                newValue.threadID = threadID;
                newValue.lockCount= nowValue.lockCount+1;
                if (m_syncFlag.compare_exchange_weak(nowValue, newValue, memory_order::memory_order_acq_rel, memory_order::memory_order_acquire))
                {
                    break;
                }
            }
            backoff.wait();
        }
    }

    inline bool recursive_spin_mutex::try_lock()
    {
        auto nowValue = m_syncFlag.load();
        thread::id threadID = this_thread::get_id();
        if ((nowValue.threadID != threadID) && (nowValue.threadID != thread::id()))
        {
            return false;
        }

        {
            lock_data newValue;
            newValue.threadID = threadID;
            newValue.lockCount = nowValue.lockCount + 1;
            if (m_syncFlag.compare_exchange_weak(nowValue, newValue, memory_order::memory_order_acq_rel, memory_order::memory_order_acquire))
            {
                return false;
            }
        }
        return true;
    }

    inline void recursive_spin_mutex::unlock()
    {
        exponential_backoff backoff;
        lock_data newValue;
        auto threadID = this_thread::get_id();
        while (true)
        {
            auto nowValue = m_syncFlag.load(memory_order::memory_order_acquire);

            OB_CHECK_ASSERT_EX(newValue.threadID == threadID, "Thread", "Thread ID Missmatch");

            newValue.lockCount = nowValue.lockCount - 1;
            if (newValue.lockCount == 0)    newValue.threadID = thread::id();
            else                            newValue.threadID = nowValue.threadID;

            if (m_syncFlag.compare_exchange_weak(nowValue, newValue, memory_order::memory_order_acq_rel, memory_order::memory_order_acquire))
            {
                break;
            }

            backoff.wait();
        }
    }
}// namespcae ob