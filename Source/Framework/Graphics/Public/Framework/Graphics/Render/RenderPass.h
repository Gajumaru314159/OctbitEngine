//***********************************************************
//! @file
//! @brief		
//! @author		Gajumaru
//***********************************************************
#pragma once

namespace ob::graphics {

	/*
	enum class FGResourceType {
		Dummy,
		Texture,
		Buffer,
	};

	struct FGRenderPassRequirement {
		String			name;
		FGResourceType	type;
	};

	struct RenderPassRequirements {
		Vector<FGRenderPassRequirement> inputs;
		Vector<FGRenderPassRequirement> outputs;
	};
	*/

	class RenderPass {
	public:
		virtual ~RenderPass() = default;
	};

}