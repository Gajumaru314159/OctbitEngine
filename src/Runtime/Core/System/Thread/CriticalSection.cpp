//***********************************************************
//! @file
//! @brief		クリティカルセクション
//! @author		Gajumaru
//***********************************************************

#include <Core/System/Thread/CriticalSection.hpp>

namespace ob
{

    //-----------------------------------------------------------
    // @brief  オブジェクトの初期化
    // 
    // @retval TRUE    成功
    // @retval FALSE   失敗
    //-----------------------------------------------------------
    b32 CriticalSection::Startup()
    {
        if (m_isStartuped)return FALSE;
        if (m_object.Startup())return FALSE;

        m_isStartuped = TRUE;

        return TRUE;
    }


    //-----------------------------------------------------------
    // @brief オブジェクトの終了
    //-----------------------------------------------------------
    void CriticalSection::Cleanup()
    {
        if (m_isStartuped == FALSE)return;
        m_object.Cleanup();
        m_isStartuped = FALSE;
    }

    
}// namespace ob