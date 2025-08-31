//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Profile/Profile.h>
#include <Framework/Core/Utility/Noncopyable.h>

#if OB_DEBUG_SPIN_LOCK
#	define OB_DEBUG_SPIN_LOCK_CONTEX(context)	context
#else
#	define OB_DEBUG_SPIN_LOCK_CONTEX(contex)	/**/
#endif

namespace ob::core {

    //! @brief  スピンロック
    class SpinLock : Noncopyable {
    public:

        //! @brief  コンストラクタ
        SpinLock();


        //! @brief  デストラクタ
        ~SpinLock();


        //! @brief  ロックを取得する
        void lock();


        //! @brief  ロックを手放す
        void unlock();

    private:

        struct Impl {
            std::atomic<bool> m_lock;
            OB_DEBUG_SPIN_LOCK_CONTEX(
                std::atomic<unsigned int> m_threadId{ 0 };
            )
            void lock();
            void unlock();
        };

        OB_PROFILE_LOCK(Impl,m_impl);

    };

}