//***********************************************************
//! @file       
//! @brief		指数関数的後退
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Runtime/Foundation/Base/Fwd.h>
#include <Runtime/Foundation/Template/chrono.h>
#include "thread.h"


namespace ob
{

    
    //@―---------------------------------------------------------------------------
    //! @brief      指数関数的後退
    //! 
    //! @details    スレッドの待機時間を指数関数的に増やす。
    //@―---------------------------------------------------------------------------
    class exponential_backoff
    {
    public:

        //@―---------------------------------------------------------------------------
        //! @brief      コンストラクタ
        //@―---------------------------------------------------------------------------
        exponential_backoff(s32 spinCount=400)
            :m_count(1)
            ,m_spinCount(spinCount)
        {}


        //@―---------------------------------------------------------------------------
        //! @brief      同期待ち
        //@―---------------------------------------------------------------------------
        void wait()
        {
            if (m_spinCount <= 0)
            {
                // TODO SwitchToThreadを用いた処理に変更
                this_thread::sleep_for(chrono::microseconds(m_count));
                if (m_count <= s_maxLoopCount)
                {
                    m_count <<= 1;
                }
            }
            else
            {
                --m_spinCount;
            }
        }


        //@―---------------------------------------------------------------------------
        //! @brief      待機時間のリセット
        //@―---------------------------------------------------------------------------
        void reset()
        {
            m_count = 1;
        }

    private:

        static const s32 s_maxLoopCount = 32;
        s32 m_count;
        s32 m_spinCount;

    };

}// namespcae ob