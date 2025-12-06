//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Log/LogMacro.h>
#include <Framework/Core/Log/Assertion.h>
#include <Framework/Core/Template/Container/Vector.h>
#include <limits>

namespace ob::core {

	template<class T, class TIndex = uint16_t, class TDataVector = Vector<T>, class TIndexVector = Vector<TIndex>>
	class IndexedVector {
	public:
		static constexpr TIndex NoFreeSlot = std::numeric_limits<TIndex>::max();
	public:
		void reserve(size_t size) {
			m_indices.reserve(size);
			m_data.reserve(size);
			m_free.reserve(size);
		}

		void clear() {
			m_indices.clear();
			m_data.clear();
			m_free.clear();
		}

		TIndex push() {

			// 空き領域がある場合はそこを使用
			if (!m_free.empty()) {
				TIndex index = m_free.back();
				m_free.pop_back();
				return index;
			}

			TIndex index = static_cast<TIndex>(m_indices.size());

			// 枯渇
			if (index == NoFreeSlot) {
				LOG_FATAL("インデックスが最大数{}を超えました",index);
				return NoFreeSlot;
			}

			m_indices.push_back(index);

			return index;
		}

		void erase(TIndex index) {
			TIndex dataIndex = m_indices.at(index);

			// 後のデータを空き領域に詰める
			m_data.at(dataIndex) = m_data.back();
			m_data.pop_back();

			// 空き領域に追加
			m_free.push_back(index);
		}

		void erase(T* data) {
			if(data == nullptr) {
				return;
			}
			auto dataIndex = data - m_data.data();

			// このコンテナで管理されているデータではない
			if (dataIndex < 0 || m_data.size() <= dataIndex) {
				return;
			}

			auto itr = std::find(m_indices.begin(), m_indices.end(), dataIndex);
			OB_ASSERT_EXPR(itr != m_indices.end());
			auto index = std::distance(m_indices.beginn(),itr);

			erase(index);
		}

		T& at(TIndex index) {
			return m_data.at(m_indices.at(index));
		}
		const T& at(TIndex index) const {			
			return m_data.at(m_indices.at(index));
		}

		size_t size() const {
			return m_data.size();
		}
		T* data() {
			return m_data.data();
		}
		const T* data() const {
			return m_data.data();
		}

		TDataVector& container() { return m_data; }
		const TDataVector& container() const { return m_data; }

	private:
		TIndexVector m_indices;
		TIndexVector m_free;
		TDataVector  m_data;
	};

}