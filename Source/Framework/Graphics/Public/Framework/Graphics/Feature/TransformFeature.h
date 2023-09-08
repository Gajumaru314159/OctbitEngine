//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphics/CameraType.h>
#include <Framework/Graphics/Render/RenderView.h>
#include <Framework/Core/Math/Transform.h>
#include <Framework/RHI/Forward.h>
#include <Framework/RHI/Texture.h>

namespace ob::graphics {

	template<typename T>
	class FreeList {
	public:
		using index_type = u32;
	public:

		FreeList() {
			m_availableIndex = 0;
		}

		void resize(size_t size) {

			auto index = m_availableIndex;
			auto last = m_items.size();
			m_items.resize(size);

			for (auto i = last; i < size; ++i) {
				m_items[i].next = index;
				index = i;
			}

			m_availableIndex = index;
		}

		index_type reserve() {

			if (m_availableIndex == s_end) {
				resize((m_items.size() + 1) * 3 / 2);
			}

			auto& item = m_items.at(m_availableIndex);
			auto index = m_availableIndex;
			m_availableIndex = item.next;
			return index;
		}

		T& at(index_type index) {
			return m_items.at(index).value;
		}

	private:
		union Item {
			T value;
			index_type next;
		};
	private:
		static index_type s_end{-1};
		index_type m_availableIndex;
		std::vector<Item> m_items;
	};


	enum class TransformId{};

	class TransformFeature {
	public:

		TransformId reserve();

		void release(TransformId);

		void setTransform(TransformId, const Transform&);
		void getTransform(TransformId)const;

	private:
		FreeList<Matrix> m_objectToWorldTransforms;
		FreeList<Matrix> m_objectToWorldTransformsPrev;
		FreeList<Matrix> m_objectToWorldInverseTransforms;

	};



}