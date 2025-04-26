//***********************************************************
//! @file
//! @brief		RHI・モジュール・コンフィグ(DirectX12)
//! @author		Gajumaru
//***********************************************************

namespace ob::rhi::dx12 {

	struct DirectX12RHIConfig {
        bool enablePIX = false;
        bool enableDebugLayer = false;
        bool breakWithWarning = false;
        bool enableDirectStorage = true;
        bool enableDirectStorageDebug = false;
	};

}