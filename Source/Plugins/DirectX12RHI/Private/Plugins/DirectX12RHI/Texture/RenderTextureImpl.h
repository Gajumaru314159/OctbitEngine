//***********************************************************
//! @file
//! @brief		レンダーテクスチャ実装(DirectX12)
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/IRenderTexture.h>
#include <Plugins/DirectX12RHI/Descriptor/DescriptorHandle.h>

//===============================================================
// 前方宣言
//===============================================================
namespace ob::rhi::dx12 {
    class DeviceImpl;
    class TextureImpl;
}


//===============================================================
// クラス定義
//===============================================================
namespace ob::rhi::dx12 {

    //@―---------------------------------------------------------------------------
    //! @brief      レンダーテクスチャ実装(DirectX12)
    //! 
    //! @details    描画可能なテクスチャ。
    //@―---------------------------------------------------------------------------
    class RenderTextureImpl :public rhi::IRenderTexture {
    public:

        //@―---------------------------------------------------------------------------
        //! @brief      コンストラクタ
        //@―---------------------------------------------------------------------------
        RenderTextureImpl(DeviceImpl& rDevice, const RenderTextureDesc& desc);


        //@―---------------------------------------------------------------------------
        //! @brief      デストラクタ
        //@―---------------------------------------------------------------------------
        ~RenderTextureImpl();


        //@―---------------------------------------------------------------------------
        //! @brief  妥当な状態か
        //@―---------------------------------------------------------------------------
        bool isValid()const override;


        //@―---------------------------------------------------------------------------
        //! @brief  サイズ
        //@―---------------------------------------------------------------------------
        Size size()const override;


        //@―---------------------------------------------------------------------------
        //! @brief      テクスチャ・フォーマットを取得
        //@―---------------------------------------------------------------------------
        TextureFormat format()const override;


        //@―---------------------------------------------------------------------------
        //! @brief      ミップレベルを取得
        //@―---------------------------------------------------------------------------
        s32 mipLevels()const override;


    public:

        D3D12_CPU_DESCRIPTOR_HANDLE getCPUHandle()const;
        D3D12_GPU_DESCRIPTOR_HANDLE getGPUHandle()const;

        D3D12_VIEWPORT getViewport()const;
        D3D12_RECT getScissorRect()const;

    private:

        void onNameChanged()override;


    private:

        const RenderTextureDesc     m_desc;

        ComPtr<ID3D12Resource>      m_resource;     //!< リソース
        DescriptorHandle            m_hSRV;         //!< デスクリプタハンドル
        DescriptorHandle            m_hRTV;         //!< デスクリプタハンドル


        D3D12_VIEWPORT m_viewport;                      //!< ビューポート
        D3D12_RECT m_scissorRect;                       //!< シザー矩形

        bool m_initialized =false;
    };

}// namespace ob::rhi::dx12