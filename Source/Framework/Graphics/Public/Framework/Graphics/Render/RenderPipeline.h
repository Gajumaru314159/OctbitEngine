//***********************************************************
//! @file
//! @brief		
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphics/Forward.h>
#include <Framework/Graphics/Render/RenderPassBuilder.h>

namespace ob::graphics {

	struct RenderPipeline {
		String						name;
		RenderPassBuilder			builer;
		Vector<RenderView*>			views;
	};

	class RenderViewList {
	public:
		using container = Vector<RenderView*>;
	public:
		class iterator {
		public:
			iterator(container::const_iterator itr)
				: m_itr(itr) {}
			iterator& operator++() { ++m_itr; return *this; }
			iterator operator++(int) { iterator tmp = *this; ++(*this); return tmp; }
			bool operator!=(const iterator& other) const { return m_itr != other.m_itr; }
			bool operator==(const iterator& other) const { return m_itr == other.m_itr; }
			RenderView& operator*() const { return **m_itr; }
		private:
			container::const_iterator m_itr;
		};
	public:
		RenderViewList(const Vector<RenderView*>& view) : m_views(view) {}
		iterator begin() const { return iterator(m_views.begin()); }
		iterator end() const { return iterator(m_views.end()); }
		size_t size() const { return m_views.size(); }
		RenderView& operator[](size_t index) const { return *m_views.at(index); }
	private:
		Vector<RenderView*> m_views;
	};

	class IRenderPipeline {
	public:
		virtual ~IRenderPipeline() = default;
		virtual void render(FG& fg, RenderViewList& views) = 0;
	};

}