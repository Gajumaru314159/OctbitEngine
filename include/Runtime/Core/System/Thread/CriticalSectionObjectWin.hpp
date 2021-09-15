//***********************************************************
//! @file
//! @brief		クリティカルセクションオブジェクト(Windows用)
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Core/Base/Fwd.hpp>

#include <Windows.h>

#ifdef OB_OS_WIN

namespace ob
{

    //-----------------------------------------------------------
    //! @brief クリティカルセクションオブジェクト
    //-----------------------------------------------------------
    class OB_API CriticalSectionObject
    {
    public:

        b32     Startup();          // オブジェクトの初期化
        void    Cleanup();          // オブジェクトの終了

        void    Begin();            // クリティカルセクションの開始
        void    End();              // クリティカルセクションの終了

    private:

        CRITICAL_SECTION    m_criticalSection;

    };






    //===============================================================
    // インライン関数
    //===============================================================

    //-----------------------------------------------------------
    // @brief          オブジェクトの初期化
    // 
    // @retval TRUE    成功
    // @retval FALSE   失敗
    //-----------------------------------------------------------
    inline b32 CriticalSectionObject::Startup()
    {
        InitializeCriticalSection(&m_criticalSection);
        SetCriticalSectionSpinCount(&m_criticalSection, 4000);
        return TRUE;
    }



    //-----------------------------------------------------------
    // @brief オブジェクトの終了
    //-----------------------------------------------------------
    inline void CriticalSectionObject::Cleanup()
    {
        DeleteCriticalSection(&m_criticalSection);
    }



    //-----------------------------------------------------------
    // @brief クリティカルセクションの開始
    //-----------------------------------------------------------
    inline void CriticalSectionObject::Begin()
    {
        EnterCriticalSection(&m_criticalSection);
    }


    //-----------------------------------------------------------
    // @brief クリティカルセクションの終了
    //-----------------------------------------------------------
    inline void CriticalSectionObject::End()
    {
       LeaveCriticalSection(&m_criticalSection);

    }

}//namespace ob

#endif// OB_OS_WIN