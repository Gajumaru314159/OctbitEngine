//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include <Runtime/Graphic/Public/System.h>
#include <Runtime/GraphicD3D12/Device/D3D12Device.h>

namespace ob {
    namespace graphic {

        //@―---------------------------------------------------------------------------
        //! @brief  コンストラクタ
        //@―---------------------------------------------------------------------------
        System::System(GraphicAPI api) {
            
            if (api == GraphicAPI::D3D12) {
#if defined(OS_WINDOWS)
                m_device = make_unique<D3D12Device>();
#endif
            } else if(api==GraphicAPI::Vulkan){
                OB_NOTIMPLEMENTED();
            }
            
            if(m_device.get()==nullptr){
                OB_NOTIMPLEMENTED();
            }
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

    }// namespace graphic
}// namespace ob