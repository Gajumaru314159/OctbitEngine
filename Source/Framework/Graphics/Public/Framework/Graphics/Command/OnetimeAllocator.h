//***********************************************************
//! @file
//! @brief		
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/Forward.h>

namespace ob::graphics {

	struct OnetimeAllocatorDesc {
		size_t blockSize = 1'000'000;
	};

	//@―---------------------------------------------------------------------------
	//! @brief  1フレームのみ使用可能な線形アロケータ
	//@―---------------------------------------------------------------------------
	class OnetimeAllocator {
	public:
		OnetimeAllocator(const OnetimeAllocatorDesc& desc)
			: m_desc(desc)
		{
		}

		void* allocate(size_t n) {

			if (m_storage.front().size() <= m_top + n) {
				m_storage.emplace_front(std::max(m_desc.blockSize,n));
				m_top = 0;
			}

			auto top = m_top;
			auto result = m_storage.front().data() + top;
			m_top += n;

			return result;
		}

		template<class T>
		T* allocate() {
			return reinterpret_cast<T*>(allocate(align_up(sizeof(T), alignof(T))));
		}

		template<class T>
		T* allocate(size_t n) {
			return reinterpret_cast<T*>(allocate(align_up(sizeof(T)*n, alignof(T))));
		}

		void reset() {

			size_t size = 0;
			for (auto& blob : m_storage) {
				size += blob.size();
			}

			m_storage.clear();
			m_storage.emplace_front(size);

			m_top = 0;
		}

	private:
		OnetimeAllocatorDesc m_desc;

		List<Blob>	m_storage;
		size_t		m_top;

	};

}