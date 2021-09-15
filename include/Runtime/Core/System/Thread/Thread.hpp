//***********************************************************
//! @file
//! @brief		スレッド
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Core/Base/Fwd.hpp>

namespace ob
{

    //-----------------------------------------------------------
    

    class Thread
    {
    public:

        typedef void (*ThreadFunc)(void*);

    private:

        class Impl;

    public:

        Thread();
        Thread(Thread&& src);

        ~Thread();

        Thread& operator=(Thread&& rhs)OB_NOEXCEPT;

        OB_DISALLOW_COPY(Thread);


        void Release();
        void Init(ThreadFunc pFunc,void *pArgs/*,const ThreadDesc& desc*/);

        void Join();
        b32 IsEmpty();


    private:

        Impl* m_pImpl;

    };






    //===============================================================
    // インライン関数
    //===============================================================

    //-----------------------------------------------------------


}// namespcae ob