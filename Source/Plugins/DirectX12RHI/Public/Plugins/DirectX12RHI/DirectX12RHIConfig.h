//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************

namespace ob::rhi {

	struct DirectX12RHIConfig {
        bool enablePIX = false;
        bool enableDebugLayer = false;
        bool breakWithWarning = false;
        bool enableDirectStorage = true;
        bool enableDirectStorageDebug = false;
	};

}