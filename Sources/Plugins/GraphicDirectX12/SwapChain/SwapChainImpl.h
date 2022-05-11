﻿//***********************************************************
//! @file
//! @brief		スワップ・チェーン実装(DirectX12)
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphic/Interface/ISwapChain.h>
#include <Plugins/GraphicDirectX12/Descriptor/DescriptorHandle.h>

//===============================================================
// 前方宣言
//===============================================================
namespace ob::graphic::dx12 {
    class DeviceImpl;
    class ITexture;
}


//===============================================================
// クラス宣言
//===============================================================
namespace ob::graphic::dx12 {

    //@―---------------------------------------------------------------------------
    //! @brief  説明
    //@―---------------------------------------------------------------------------
    class SwapChainImpl :public graphic::ISwapChain{
    public:

        //===============================================================
        // コンストラクタ / デストラクタ
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief  コンストラクタ
        //@―---------------------------------------------------------------------------
        SwapChainImpl(DeviceImpl& rDevice, const SwapchainDesc& desc);


        //@―---------------------------------------------------------------------------
        //! @brief  デストラクタ
        //@―---------------------------------------------------------------------------
        ~SwapChainImpl();


        //@―---------------------------------------------------------------------------
        //! @brief  妥当なオブジェクトか
        //@―---------------------------------------------------------------------------
        bool isValid()const override;


        //===============================================================
        // ゲッター
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief  バックバッファの数を取得
        //@―---------------------------------------------------------------------------
        const SwapchainDesc& getDesc()const noexcept override;


        //===============================================================
        // 更新
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief  バックバッファのサイズを変更
        //@―---------------------------------------------------------------------------
        bool resizeBackBuffer(const Size& size) override;


        //@―---------------------------------------------------------------------------
        //! @brief      更新
        //! 
        //! @details    表示するテクスチャを次のバックバッファにします。
        //@―---------------------------------------------------------------------------
        void update(const Texture& texture) override;


        //@―---------------------------------------------------------------------------
        //! @brief      デスクリプタCPUハンドルを取得
        //@―---------------------------------------------------------------------------
        D3D12_CPU_DESCRIPTOR_HANDLE getCpuHandle()const;


        //@―---------------------------------------------------------------------------
        //! @brief      デスクリプタGPUハンドルを取得
        //@―---------------------------------------------------------------------------
        D3D12_GPU_DESCRIPTOR_HANDLE getGpuHandle()const;


        D3D12_VIEWPORT getViewport()const;
        D3D12_RECT getScissorRect()const;

    private:

        //@―---------------------------------------------------------------------------
        //! @brief      レンダーテクスチャを初期化
        //@―---------------------------------------------------------------------------
        bool createSwapChain(DeviceImpl& rDevice);

        //@―---------------------------------------------------------------------------
        //! @brief      レンダーテクスチャを初期化
        //@―---------------------------------------------------------------------------
        bool createBuffer(DeviceImpl& rDevice);


        //@―---------------------------------------------------------------------------
        //! @brief      カラースペースを設定
        //@―---------------------------------------------------------------------------
        bool setColorSpace();


    private:

        SwapchainDesc m_desc;

        ComPtr<IDXGISwapChain4> m_swapchain;            //!< スワップチェイン
        vector<ComPtr<ID3D12Resource>> m_buffers;       //!< バックバッファ

        D3D12_VIEWPORT m_viewport;                      //!< ビューポート
        D3D12_RECT m_scissorRect;                       //!< シザー矩形

        TextureFormat m_displayViewFormat;              //!< ディスプレイ・ビューフォーマット
        DXGI_FORMAT m_nativeDisplayViewFormat;          //!< ディスプレイ・ビューフォーマット
        DXGI_FORMAT m_nativeSwapChainFormat;            //!< ディスプレイ・ビューフォーマット

        DescriptorHandle m_hRTV;
        s32 m_frameIndex;

        UINT m_syncInterval;

        bool m_initialized = false;



    };

}// namespcae ob::graphic::dx12