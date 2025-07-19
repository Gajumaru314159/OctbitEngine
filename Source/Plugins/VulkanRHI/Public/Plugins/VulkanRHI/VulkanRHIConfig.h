//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#pragma once

namespace ob::rhi {

	struct VulkanRHIConfig {
		bool enableDebugLayer = false;
		LogLevel logLevel = LogLevel::Warning;
	};

}