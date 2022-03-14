//***********************************************************
//! @file
//! @brief		テクスチャ実装(DirectX12)
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphic/Interface/ITexture.h>
#include <Framework/Graphic/Types/TextureDesc.h>

namespace ob::graphic::dx12 {
    class DeviceImpl;
}

namespace ob::graphic::dx12 {

    class TextureImpl :public ITexture {
    public:

        //@―---------------------------------------------------------------------------
        //! @brief      コンストラクタ
        //@―---------------------------------------------------------------------------
        TextureImpl(DeviceImpl& rDevice, const TextureDesc& desc, StringView name);


        //@―---------------------------------------------------------------------------
        //! @brief      テクスチャ・サイズを取得
        //@―---------------------------------------------------------------------------
        Size size() override;


        //@―---------------------------------------------------------------------------
        //! @brief      テクスチャ・フォーマットを取得
        //@―---------------------------------------------------------------------------
        TextureFormat format()const override;


        //@―---------------------------------------------------------------------------
        //! @brief      テクスチャ使用法を取得
        //@―---------------------------------------------------------------------------
        ResourceUsage usage()const override;


        //@―---------------------------------------------------------------------------
        //! @brief      ミップマップを持っているか
        //@―---------------------------------------------------------------------------
        bool hasMipmap()const override;

        //-------------------
        ID3D12DescriptorHeap* getSRVPtr();


    private:

        TextureDesc             m_desc;
        ComPtr<ID3D12Resource>  m_resource;     //!< D3D12のデバイス本体
        //vector<Texture> m_mipmaps;              //!< ミップマップ


    };

}// namespace ob::graphic::dx12