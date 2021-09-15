//***********************************************************
//! @file
//! @brief		STL
//! @author		Gajumaru
//***********************************************************
#include <Core/Template/STL.hpp>
#include <Core/Memory/Heap/OSHeap.hpp>

namespace ob
{
    

	//-----------------------------------------------------------

	STLManager::STLManager()
	{
		m_alloc = new OSHeap();
        m_canChangeAlloc = TRUE;
	}

    Heap* STLManager::GetAlloc()
    {
        m_canChangeAlloc = FALSE;
        return m_alloc;
    }

    b32 STLManager::SetAlloc(Heap* alloc)
    {
        if (m_canChangeAlloc)return FALSE;
        if (alloc==nullptr)return FALSE;
        m_alloc = alloc;
        return TRUE;
    }


    EASTLAllocator::EASTLAllocator(const char* pName)
    {
        m_pName = pName;
    }

    EASTLAllocator::EASTLAllocator(const EASTLAllocator& x, const char* pName)
    {
        *this = x;
        m_pName = pName;
    }

    void* EASTLAllocator::allocate(size_t n, int flags)
    {
        return STLManager::Instance().GetAlloc()->Allocate(n);
    }

    void* EASTLAllocator::allocate(size_t n, size_t alignment, size_t offset, int flags)
    {
        return STLManager::Instance().GetAlloc()->Allocate(n, alignment);
    }

    void  EASTLAllocator::deallocate(void* p, size_t n)
    {
        return STLManager::Instance().GetAlloc()->Deallocate(p);
    }

    const char* EASTLAllocator::get_name() const
    {
        return m_pName;
    }
    void EASTLAllocator::set_name(const char* pName)
    {
        m_pName = pName;
    }

    bool operator==(const EASTLAllocator& a, const EASTLAllocator& b)
    {
        return true;
    }

    bool operator!=(const EASTLAllocator& a, const EASTLAllocator& b)
    {
        return false;
    }

}// namespace ob