﻿//***********************************************************
//! @file
//! @brief		テクスチャ実装(DirectX12)
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphic/Interface/ITexture.h>
#include <Framework/Graphic/Types/TextureDesc.h>
#include <Framework/Core/Misc/BlobView.h>
#include <Plugins/GraphicDirectX12/Descriptor/DescriptorHandle.h>



//===============================================================
// 前方宣言
//===============================================================
namespace ob::graphic::dx12 {
    class DeviceImpl;
}


//===============================================================
// クラス定義
//===============================================================
namespace ob::graphic::dx12 {

    class TextureImpl :public graphic::ITexture {
    public:

        //@―---------------------------------------------------------------------------
        //! @brief      コンストラクタ
        //@―---------------------------------------------------------------------------
        TextureImpl(DeviceImpl& rDevice, const TextureDesc& desc);

        //@―---------------------------------------------------------------------------
        //! @brief      コンストラクタ
        //@―---------------------------------------------------------------------------
        TextureImpl(DeviceImpl& rDevice, BlobView blob);


        //@―---------------------------------------------------------------------------
        //! @brief      デストラクタ
        //@―---------------------------------------------------------------------------
        ~TextureImpl();


        //@―---------------------------------------------------------------------------
        //! @brief  妥当な状態か
        //@―---------------------------------------------------------------------------
        bool isValid()const override;


        //@―---------------------------------------------------------------------------
        //! @brief      定義を取得
        //@―---------------------------------------------------------------------------
        const TextureDesc& getDesc() const override;


    public:

        //@―---------------------------------------------------------------------------
        //! @brief      リソースを取得
        //@―---------------------------------------------------------------------------
        ID3D12Resource* getResource()const;


        //@―---------------------------------------------------------------------------
        //! @brief      シェーダリソースビューを生成
        //@―---------------------------------------------------------------------------
        void createSRV(D3D12_CPU_DESCRIPTOR_HANDLE handle)const;


    private:

        class DeviceImpl& m_device;

        TextureDesc                 m_desc;         //!< 定義
        ComPtr<ID3D12Resource>      m_resource;     //!< リソース
        DescriptorHandle            m_hSRV;         //!< デスクリプタハンドル

    };

}// namespace ob::graphic::dx12



//===============================================================
// インライン
//===============================================================
namespace ob::graphic::dx12 {

    //@―---------------------------------------------------------------------------
    //! @brief      リソースを取得
    //@―---------------------------------------------------------------------------
    inline ID3D12Resource* TextureImpl::getResource() const {
        return m_resource.Get();
    }


    //@―---------------------------------------------------------------------------
    //! @brief      定義を取得
    //@―---------------------------------------------------------------------------
    inline const TextureDesc& TextureImpl::getDesc() const {
        return m_desc;
    }

}// namespace ob::graphic::dx12
