//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include <Framework/Graphic/System.h>
#include <Framework/Platform/Module/ModuleManager.h>
#include <Framework/Graphic/Module/IGraphicModule.h>

namespace ob::graphic {

    //@―---------------------------------------------------------------------------
    //! @brief  コンストラクタ
    //@―---------------------------------------------------------------------------
    System::System(GraphicAPI api) {
        if (api == GraphicAPI::D3D12) {
#if defined(OS_WINDOWS)
            auto pModule = platform::ModuleManager::Get().LoadModule<IGraphicModule>(TC("GraphicD3D12"));
            
            m_device = std::unique_ptr<I>(pModule->CreateDevice(FeatureLevel::Default));
#endif
        } else if (api == GraphicAPI::Vulkan) {
            OB_NOTIMPLEMENTED();
        }

        if (m_device.get() == nullptr) {
            OB_NOTIMPLEMENTED();
        }
    }


    //@―---------------------------------------------------------------------------
    //! @brief  デストラクタ
    //@―---------------------------------------------------------------------------
    System::~System() {

    }


    //@―---------------------------------------------------------------------------
    //! @brief  スワップチェーンの生成
    //@―---------------------------------------------------------------------------
    Ref<Swapchain> System::createSwapchain(const SwapchainDesc& desc) {
        return m_device->createSwapchain(desc);
    }

    //@―---------------------------------------------------------------------------
    //! @brief  コマンドリストの生成
    //@―---------------------------------------------------------------------------
    Ref<CommandList> System::createCommandList(CommandListType type) {
        return m_device->createCommandList(type);
    }

}// namespace pb::graphic