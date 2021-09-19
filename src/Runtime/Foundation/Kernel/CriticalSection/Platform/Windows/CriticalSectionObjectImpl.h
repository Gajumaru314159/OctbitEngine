﻿//***********************************************************
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

    //@―-------------------------------------------------------------------------------
    //! @brief クリティカルセクションオブジェクト
    //@―-------------------------------------------------------------------------------
    class CriticalSectionObject
    {
    public:

        //@―-------------------------------------------------------------------------------
        //! @brief          オブジェクトの初期化
        //! 
        //! @retval true    成功
        //! @retval false   失敗
        //@―-------------------------------------------------------------------------------
        bool Startup();


        //@―-------------------------------------------------------------------------------
        //! @brief オブジェクトの終了
        //@―-------------------------------------------------------------------------------
        void Cleanup();


        //@―-------------------------------------------------------------------------------
        //1 @brief クリティカルセクションの開始
        //@―-------------------------------------------------------------------------------
        void Begin(); 


        //@―-------------------------------------------------------------------------------
        //! @brief クリティカルセクションの終了
        //@―-------------------------------------------------------------------------------
        void End();

    private:

        CRITICAL_SECTION    m_criticalSection;

    };






    //===============================================================
    // インライン関数
    //===============================================================
    //! @cond

    //@―-------------------------------------------------------------------------------
    //! @brief          オブジェクトの初期化
    //! 
    //! @retval TRUE    成功
    //! @retval FALSE   失敗
    //@―-------------------------------------------------------------------------------
    inline bool CriticalSectionObject::Startup()
    {
        InitializeCriticalSection(&m_criticalSection);
        SetCriticalSectionSpinCount(&m_criticalSection, 4000);
        return true;
    }



    //@―-------------------------------------------------------------------------------
    //! @brief オブジェクトの終了
    //@―-------------------------------------------------------------------------------
    inline void CriticalSectionObject::Cleanup()
    {
        DeleteCriticalSection(&m_criticalSection);
    }



    //@―-------------------------------------------------------------------------------
    //! @brief クリティカルセクションの開始
    //@―-------------------------------------------------------------------------------
    inline void CriticalSectionObject::Begin()
    {
        EnterCriticalSection(&m_criticalSection);
    }


    //@―-------------------------------------------------------------------------------
    //! @brief クリティカルセクションの終了
    //@―-------------------------------------------------------------------------------
    inline void CriticalSectionObject::End()
    {
       LeaveCriticalSection(&m_criticalSection);

    }


    //! @endcond
}//namespace ob

#endif// OS_WIN