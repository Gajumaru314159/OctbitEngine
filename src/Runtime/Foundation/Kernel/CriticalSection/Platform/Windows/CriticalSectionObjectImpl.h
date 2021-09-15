//***********************************************************
//! @file
//! @brief		クリティカルセクションオブジェクト(Windows用)
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Foundation/Base/Fwd.h>
#include <Windows.h>

#ifdef OS_WINDOWS

namespace ob
{

    //-----------------------------------------------------------------------------
    //! @brief クリティカルセクションオブジェクト
    //-----------------------------------------------------------------------------
    class CriticalSectionObject
    {
    public:

        bool     Startup();          // オブジェクトの初期化
        void    Cleanup();          // オブジェクトの終了

        void    Begin();            // クリティカルセクションの開始
        void    End();              // クリティカルセクションの終了

    private:

        CRITICAL_SECTION    m_criticalSection;

    };






    //===============================================================
    // インライン関数
    //===============================================================

    //-----------------------------------------------------------------------------
    // @brief          オブジェクトの初期化
    // 
    // @retval TRUE    成功
    // @retval FALSE   失敗
    //-----------------------------------------------------------------------------
    inline bool CriticalSectionObject::Startup()
    {
        InitializeCriticalSection(&m_criticalSection);
        SetCriticalSectionSpinCount(&m_criticalSection, 4000);
        return true;
    }



    //-----------------------------------------------------------------------------
    // @brief オブジェクトの終了
    //-----------------------------------------------------------------------------
    inline void CriticalSectionObject::Cleanup()
    {
        DeleteCriticalSection(&m_criticalSection);
    }



    //-----------------------------------------------------------------------------
    // @brief クリティカルセクションの開始
    //-----------------------------------------------------------------------------
    inline void CriticalSectionObject::Begin()
    {
        EnterCriticalSection(&m_criticalSection);
    }


    //-----------------------------------------------------------------------------
    // @brief クリティカルセクションの終了
    //-----------------------------------------------------------------------------
    inline void CriticalSectionObject::End()
    {
       LeaveCriticalSection(&m_criticalSection);

    }

}//namespace ob

#endif// OS_WIN