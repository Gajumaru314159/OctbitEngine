//***********************************************************
//! @file
//! @brief		グラフィック・システム
//! @author		Gajumaru
//***********************************************************
#pragma once
#include "Type/GraphicAPI.h"
#include "Device.h"

namespace ob {
    namespace graphic {

        //@―---------------------------------------------------------------------------
        //! @brief  説明
        //@―---------------------------------------------------------------------------
        class System:public singleton<System> {
        public:

            //===============================================================
            // コンストラクタ / デストラクタ
            //===============================================================

            //@―---------------------------------------------------------------------------
            //! @brief      コンストラクタ
            //! 
            //! @details    プラットフォームにより使用できないAPIがあります。
            //!             使用できるプラットフォームは GraphicAPI を参照してください。
            //! @param api  使用するグラフィックAPIタイプ
            //@―---------------------------------------------------------------------------
            System(GraphicAPI api);

            //@―---------------------------------------------------------------------------
            //! @brief  スワップチェーンの生成
            //@―---------------------------------------------------------------------------
            Ref<Swapchain> createSwapchain(const SwapchainDesc& desc);

            //@―---------------------------------------------------------------------------
            //! @brief  コマンドリストの生成
            //@―---------------------------------------------------------------------------
            Ref<CommandList> createCommandList(CommandListType type);

        protected:

            // SwapChain
            // CommandList
            // RenderPass
            // Pipeline
            // VertexLayout
            // Buffer
            // VertexBuffer
            // IndexBuffer
            // Texture2D
            // Texture3D
            // RenderTexture
            // DepthBuffer
            // SamplerState
            // ShaderPss

            // onFlushCommandList

        private:

            unique_ptr<Device> m_device;

        };

    }// namepspace graphic
}// namespcae ob