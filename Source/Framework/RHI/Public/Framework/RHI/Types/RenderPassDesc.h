//***********************************************************
//! @file
//! @brief		バッファ定義
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/Types/TextureFormat.h>

namespace ob::rhi {

#pragma region Enum

	//@―---------------------------------------------------------------------------
	//! @brief  リソース使用法
	//@―---------------------------------------------------------------------------
	
#pragma endregion

#pragma region Flag

	//@―---------------------------------------------------------------------------
	//! @brief  バインド・フラグ
	//@―---------------------------------------------------------------------------
	
#pragma endregion

	struct AttachmentReference {

	};

	struct SubpassDependency {

	};
	
	struct SubpassDesc {
		Array<AttachmentReference> inputs;
		Array<AttachmentReference> colors;
		Array<AttachmentReference> resolves;
		Array<AttachmentReference> depth;
	};

	struct AttachmentDesc {
		TextureFormat format;
		// loadOp;
		// storeOp;
		// stencilLoadOp;
		// stencilStoreOp;
		// initialLayout;
		// finalLayout
	};

	//@―---------------------------------------------------------------------------
	//! @brief  バッファ定義
	//@―---------------------------------------------------------------------------
	struct RenderPassDesc {
		Array<AttachmentDesc>		attachments;
		Array<SubpassDesc>			subpassCount;
		Array<SubpassDependency>	dependencies;
	};

}// namespcae ob::rhi