﻿//***********************************************************
//! @file
//! @brief		可変長ハンドルマネージャ
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <memory>
#include <Framework/Foundation/Base/Common.h>
#include <Framework/Foundation/Template/container/stack.h>
/*
namespace ob
{

    //@―---------------------------------------------------------------------------
    //! @brief 可変長ハンドルマネージャ
    //@―---------------------------------------------------------------------------
    template<typename T, class TAllocator = std::allocator<T>>
    class handle_manager
    {
    public:

        class handle
        {
            friend class handle_manager<T>;
        private:
            enum
            {
                MAX_BTIS_INDEX = 16,
                MAX_BITS_MAGIC = 16,
                MAX_INDEX = (1 << MAX_BITS_INDEX) - 1,
                MAX_MAGIC = (1 << MAX_BITS_MAGIC) - 1,
            };

            struct HANDLE
            {
                unsigned index : MAX_BTIS_INDEX;
                unsigned magic : MAX_BTIS_MAGIC;
            };

        public:

            constexpr handle()noexcept;
            constexpr handle(const handle& src)noexcept;
            constexpr handle(handle&& src)noexcept;
            ~handle() = default;

            constexpr handle& oprator = (const handle & rhs)noexcept;
            constexpr handle& oprator = (handle && rhs)noexcept;

            constexpr bool operator!=(const handle& rhs)const noexcept;
            constexpr bool operator==(const handle& rhs)const noexcept;
            constexpr bool operator<(const handle& rhs)const noexcept;
            constexpr bool operator>(const handle& rhs)const noexcept;
            constexpr bool operator<=(const handle& rhs)const noexcept;
            constexpr bool operator>=(const handle& rhs)const noexcept;

            constexpr bool empty()const noexcept;
            constexpr bool clear()noexcept;

        private:

            union
            {
                HANDLE m_handle;
                // シリアライズ用変数
            };

        };

    private:

        struct node
        {
            T value;
            node* pNext;
        };

        using node_stack_type = stack<node>;

    public:

        using allocator_type = TAllocator;

        handle_manager(const allocator_type& allocator = allocator_type());
        ~handle_manager();

        OB_DISALLOW_COPY(handle_manager);
        OB_DISALLOW_MOVE(handle_manager);

    private:



    };






    //===============================================================
    // インライン関数
    //===============================================================

    //@―---------------------------------------------------------------------------


}// namespcae ob

*/