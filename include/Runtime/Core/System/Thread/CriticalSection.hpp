﻿//***********************************************************
//! @file
//! @brief		クリティカルセクション
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Core/Base/Fwd.hpp>

#include "CriticalSectionObject.hpp"

namespace ob
{
    //-----------------------------------------------------------
    //! @brief      クリティカルセクション
    //! 
    //! @details    マルチスレッドでの排他的処理を Begin() End() 間で実現する。
    //-----------------------------------------------------------
    class OB_API CriticalSection
    {
    public:

        //-----------------------------------------------------------
        // コンストラクタ / デストラクタ
        //-----------------------------------------------------------
        CriticalSection();          // デフォルトコンストラクタ
        ~CriticalSection();         // デストラクタ

        b32     Startup();          // オブジェクトの初期化
        void    Cleanup();          // オブジェクトの終了

        void    Begin();            // クリティカルセクションの開始
        void    End();              // クリティカルセクションの終了

        b32     IsValid()const;     // 有効なオブジェクトか判定

    private:

        CriticalSectionObject   m_object;
        b8 m_isStartuped;

    };

    

    //-----------------------------------------------------------
    //! @brief      スコープ付きクリティカルセクション
    //! 
    //! @details    クリティカルセクションの開始と終了をコンストラクタ/デストラクタで自動的に行う。
    //-----------------------------------------------------------
    class ScopedCriticalSection
    {
    public:
        //! @cond 
        ScopedCriticalSection(CriticalSection& cs) :m_cs(cs) { m_cs.Begin(); }  // コンストラクタ
        ~ScopedCriticalSection(){ m_cs.End(); }                                 // デストラクタ

        ScopedCriticalSection() = delete;
        //! @endcond
    private:
        CriticalSection& m_cs;

        OB_DISALLOW_COPY(ScopedCriticalSection);
    };






    //===============================================================
    // インライン関数
    //===============================================================
    
    //-----------------------------------------------------------
    // @brief デフォルトコンストラクタ
    //-----------------------------------------------------------
    inline CriticalSection::CriticalSection()
        :m_isStartuped(FALSE)
        , m_object()
    {}


    //-----------------------------------------------------------
    // @brief デストラクタ
    //-----------------------------------------------------------
    inline CriticalSection::~CriticalSection()
    {
        Cleanup();
    }


    //-----------------------------------------------------------
    // @brief      クリティカルセクションの開始
    // 
    // @details    End() が呼び出されるまで他のスレッドを停止し、他のスレッドからの同時書き込みを防ぐ。
    //-----------------------------------------------------------
    inline void CriticalSection::Begin()
    {
        m_object.Begin();
    }


    //-----------------------------------------------------------
    // @brief クリティカルセクションの終了
    //-----------------------------------------------------------
    inline void CriticalSection::End()
    {
        m_object.End();
    }


    //-----------------------------------------------------------
    // @brief          有効なオブジェクトか判定
    // 
    // @retval TRUE    有効
    // @retval FALSE   無効
    //-----------------------------------------------------------
    inline b32 CriticalSection::IsValid()const
    {
        return m_isStartuped;
    }


}// namespace ob