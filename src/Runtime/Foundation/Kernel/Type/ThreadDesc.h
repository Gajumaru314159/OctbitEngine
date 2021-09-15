//***********************************************************
//! @file
//! @brief		スレッド定義
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Foundation/Base/Fwd.h>
#include "KernelType.h"

namespace ob
{
    namespace kernel
    {

        //===============================================================
        //! @brief スレッド定義
        //===============================================================
        struct ThreadDesc
        {
        public:

            ThreadDesc(const ThreadDesc& src);                          //<! コピーコンストラクタ
            ThreadDesc(ThreadDesc&& src)=default;                       //   ムーブコンストラクタ
            ThreadDesc& operator=(const ThreadDesc& rhs);               //<! コピー代入演算子
            ThreadDesc& operator=(ThreadDesc&& rhs)=default;            //   ムーブ代入演算子

        public:

            u32             affinityMask    =0xFFFFFFFF;                //!< アフィニティマスク
            u32             stackSize       =0x2000;                    //!< スタックサイズ
            ThreadPriority  priority        =ThreadPriority::Normal;    //!< スレッド優先順序
#ifdef OB_DEBUG
            Char            name[32]        =TEXT("Unkwown");           //!< スレッド名
#endif
        };






        //===============================================================
        // インライン関数
        //===============================================================
        
        //-----------------------------------------------------------------------------
        //! @brief  コピーコンストラクタ
        //-----------------------------------------------------------------------------
        ThreadDesc::ThreadDesc(const ThreadDesc& src)
        {
            affinityMask    = src.affinityMask;
            stackSize = src.stackSize;
            priority = src.priority;
#ifdef OB_DEBUG
            memcpy(name, src.name, sizeof(name));
#endif
        }


        //-----------------------------------------------------------------------------
        //! @brief  コピー代入演算子
        //-----------------------------------------------------------------------------
        ThreadDesc& ThreadDesc::operator=(const ThreadDesc& rhs)
        {
            *this = ThreadDesc(rhs);
        }

    }// namespace kernel
}// namespcae ob