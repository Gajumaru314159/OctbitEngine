//***********************************************************
//! @file
//! @brief		レンダーテクスチャ実装(DirectX12)
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphic/Interface/IRenderTarget.h>
#include <Framework/Graphic/Types/RenderTargetDesc.h>
#include <Framework/Graphic/Texture.h>
#include <Plugins/GraphicDirectX12/Descriptor/DescriptorHandle.h>


//===============================================================
// 前方宣言
//===============================================================
namespace ob::graphic::dx12 {
    class DeviceImpl;
    class TextureImpl;
}


//===============================================================
// クラス定義
//===============================================================
namespace ob::graphic::dx12 {

    //@―---------------------------------------------------------------------------
    //! @brief      レンダーテクスチャ実装(DirectX12)
    //! 
    //! @details    描画可能なテクスチャ。
    //@―---------------------------------------------------------------------------
    class RenderTargetImpl :public graphic::IRenderTarget {
    public:

        //@―---------------------------------------------------------------------------
        //! @brief      コンストラクタ
        //@―---------------------------------------------------------------------------
        RenderTargetImpl(DeviceImpl& rDevice, const RenderTargetDesc& desc);


        //@―---------------------------------------------------------------------------
        //! @brief      デストラクタ
        //@―---------------------------------------------------------------------------
        ~RenderTargetImpl();


        //@―---------------------------------------------------------------------------
        //! @brief  妥当な状態か
        //@―---------------------------------------------------------------------------
        bool isValid()const override;


        //@―---------------------------------------------------------------------------
        //! @brief      定義を取得
        //@―---------------------------------------------------------------------------
        const RenderTargetDesc& getDesc()const override;


        //@―---------------------------------------------------------------------------
        //! @brief      カラーテクスチャ取得
        //@―---------------------------------------------------------------------------
        Texture getColorTexture(s32 index)const override;


        //@―---------------------------------------------------------------------------
        //! @brief      デプステクスチャ取得
        //@―---------------------------------------------------------------------------
        Texture getDepthTexture()const override;


        //@―---------------------------------------------------------------------------
        //! @brief      描画ターゲット枚数を取得
        //! 
        //! @details    デプス・ステンシルテクスチャは含まれません。
        //@―---------------------------------------------------------------------------
        s32 getColorTextureCount()const override;


    public:

        D3D12_CPU_DESCRIPTOR_HANDLE getColorCpuHandle(s32 index = 0)const;
        D3D12_GPU_DESCRIPTOR_HANDLE getColorGpuHandle(s32 index = 0)const;
        D3D12_CPU_DESCRIPTOR_HANDLE getDepthCpuHandle()const;
        D3D12_GPU_DESCRIPTOR_HANDLE getDepthGpuHandle()const;

        D3D12_VIEWPORT getViewport()const;
        D3D12_RECT getScissorRect()const;


    private:

        const RenderTargetDesc m_desc;

        vector<Texture> m_textures;                     //!< ターゲット・テクスチャ・リスト
        Texture m_depth;                                //!< デプス・ステンシル・テクスチャ

        DescriptorHandle m_hRTV;                        //!< レンダー・ターゲット・ビュー
        DescriptorHandle m_hDSV;                        //!< デプス・ステンシル・ビュー

        D3D12_VIEWPORT m_viewport;                      //!< ビューポート
        D3D12_RECT m_scissorrect;                       //!< シザー矩形

        bool m_initialized =false;
    };

}// namespace ob::graphic::dx12