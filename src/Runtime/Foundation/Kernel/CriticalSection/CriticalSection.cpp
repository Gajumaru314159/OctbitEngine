//***********************************************************
//! @file
//! @brief		クリティカルセクション
//! @author		Gajumaru
//***********************************************************

#include "CriticalSection.h"
namespace ob
{

    //@―-------------------------------------------------------------------------------
    // @brief  オブジェクトの初期化
    // 
    // @retval true    成功
    // @retval false   失敗
    //@―-------------------------------------------------------------------------------
    bool CriticalSection::Startup()
    {
        if (m_isStartuped)return false;
        if (!m_object.Startup())return false;

        m_isStartuped = true;

        return true;
    }


    //@―-------------------------------------------------------------------------------
    // @brief オブジェクトの終了
    //@―-------------------------------------------------------------------------------
    void CriticalSection::Cleanup()
    {
        if (m_isStartuped == false)return;
        m_object.Cleanup();
        m_isStartuped = false;
    }

    
}// namespace ob