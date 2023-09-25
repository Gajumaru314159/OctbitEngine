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
		OnetimeAllocator(const OnetimeAllocatorDesc& desc) {
			m_storage.resize(desc.blockSize);
			reset();
		}

		void* allocate(size_t n) {

			auto top = m_top.fetch_add(n);

			// TODO 自動再確保する
			OB_ASSERT(top + n < m_top.size(), "OnetimeAllocatorの最大容量を超えました。");

			return m_storage.data() + top;
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
			m_top = 0;
		}

	private:

		Array<u8> m_storage;
		Atomic<size_t> m_top;

	};

}