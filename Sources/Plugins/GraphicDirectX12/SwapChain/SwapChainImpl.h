﻿//***********************************************************
//! @file
//! @brief		スワップ・チェーン実装(DirectX12)
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphic/Interface/ISwapChain.h>

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
        SwapChainImpl(DeviceImpl& rDevice,const SwapchainDesc& desc);


        //===============================================================
        // ゲッター
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief  バックバッファの数を取得
        //@―---------------------------------------------------------------------------
        s32 getBackBufferCount()const override;


        //@―---------------------------------------------------------------------------
        //! @brief  VSyncが有効か
        //@―---------------------------------------------------------------------------
        s32 isVSyncEnabled()const override;


        //@―---------------------------------------------------------------------------
        //! @brief  HDRが有効か
        //@―---------------------------------------------------------------------------
        s32 isHdrEnabled()const override;


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
        void update() override;

    private:

        //@―---------------------------------------------------------------------------
        //! @brief      レンダーテクスチャを初期化
        //@―---------------------------------------------------------------------------
        void createSwapChain(DeviceImpl& rDevice);

        //@―---------------------------------------------------------------------------
        //! @brief      レンダーテクスチャを初期化
        //@―---------------------------------------------------------------------------
        void createBuffer(DeviceImpl& rDevice);


        //@―---------------------------------------------------------------------------
        //! @brief      カラースペースを設定
        //@―---------------------------------------------------------------------------
        void setColorSpace();



    private:

        SwapchainDesc m_desc;
        ComPtr<IDXGISwapChain4> m_swapchain = nullptr;  //!< スワップチェイン
        vector<ComPtr<ID3D12Resource>> m_buffers;                    //!< バックバッファ
        ComPtr<ID3D12DescriptorHeap> m_rtvHeaps;        //!< ディスクリプタヒープ
        D3D12_VIEWPORT m_viewport;                      //!< ビューポート
        D3D12_RECT m_scissorrect;                       //!< シザー矩形

        s32 m_frameIndex;
        TextureFormat m_displayViewFormat;              //!< ディスプレイ・ビューフォーマット
        DXGI_FORMAT m_nativeDisplayViewFormat;          //!< ディスプレイ・ビューフォーマット
        DXGI_FORMAT m_nativeSwapChainFormat;            //!< ディスプレイ・ビューフォーマット


        ComPtr<ID3D12DescriptorHeap> m_rtvHeap;                 //!< レンダー・ターゲット・ビュー

    };

}// namespcae ob::graphic::dx12