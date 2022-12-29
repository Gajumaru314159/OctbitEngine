//***********************************************************
//! @file
//! @brief		レンダーテクスチャ実装(DirectX12)
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/RenderTexture.h>
#include <Plugins/DirectX12RHI/Descriptor/DescriptorHandle.h>
#include <Plugins/DirectX12RHI/Texture/TextureImpl.h>

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
    class RenderTextureImpl :public TextureImpl, rhi::RenderTexture {
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

        const DescriptorHandle& getSRV()const { return m_hSRV; }
        const DescriptorHandle& getRTV()const { return m_hRTV; }
        const DescriptorHandle& getDSV()const { return m_hDSV; }

        D3D12_VIEWPORT getViewport()const;
        D3D12_RECT getScissorRect()const;

        auto getResource()const { return m_resource.Get(); }

    private:

        void onNameChanged()override;


    private:

        const RenderTextureDesc     m_desc;

        DescriptorHandle            m_hRTV;         //!< デスクリプタハンドル
        DescriptorHandle            m_hDSV;         //!< デスクリプタハンドル

        D3D12_VIEWPORT m_viewport;                      //!< ビューポート
        D3D12_RECT m_scissorRect;                       //!< シザー矩形

        bool m_initialized =false;
    };

}// namespace ob::rhi::dx12