//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/Types/VertexLayout.h>

namespace ob::graphics {

	class MeshSystem {
	public:

	private:
		using VertexLayout = ob::rhi::VertexLayout;

		SpinLock				m_vertexLayoutsLock;
		Vector<VertexLayout>	m_vertexLayouts;
	};

}