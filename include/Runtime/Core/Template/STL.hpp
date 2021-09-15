//***********************************************************
//! @file
//! @brief		STL
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Core/Base/Fwd.hpp>
#include <Core/Misc/Singleton.hpp>
#include <Core/Memory/Heap/Heap.hpp>

#include <EASTL/allocator.h>

namespace ob
{
    //-----------------------------------------------------------
    

    class STLManager:public Singleton<STLManager>
    {
        friend class Singleton<STLManager>;
    public:

        Heap* GetAlloc();
        b32 SetAlloc(Heap* alloc);

    protected:

        STLManager();

    private:

        Heap*  m_alloc;
        b32             m_canChangeAlloc;

    };


    class OB_API EASTLAllocator
    {
    public:
        EASTL_ALLOCATOR_EXPLICIT EASTLAllocator(const char* pName = EASTL_NAME_VAL(EASTL_ALLOCATOR_DEFAULT_NAME));
        EASTLAllocator(const EASTLAllocator& x) = default;
        EASTLAllocator(const EASTLAllocator& x, const char* pName);

        EASTLAllocator& operator=(const EASTLAllocator& x) = default;

        void* allocate(size_t n, int flags = 0);
        void* allocate(size_t n, size_t alignment, size_t offset, int flags = 0);
        void  deallocate(void* p, size_t n);

        const char* get_name() const;
        void        set_name(const char* pName);

    protected:
#if EASTL_NAME_ENABLED
        const char* m_pName;
#endif
    };

    bool operator==(const EASTLAllocator& a, const EASTLAllocator& b);
    bool operator!=(const EASTLAllocator& a, const EASTLAllocator& b);



    //===============================================================
    // インライン関数
    //===============================================================

    //-----------------------------------------------------------


}// namespcae ob