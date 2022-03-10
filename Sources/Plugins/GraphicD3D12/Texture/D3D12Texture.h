//***********************************************************
//! @file
//! @brief		グラフィックAPI
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphic/Image/Image.h>
#include <Framework/Graphic/Texture/Texture.h>
#include <Framework/Graphic/Texture/ITexture.h>


namespace ob {
    namespace graphic {

        class D3D12Texture :public ITexture {
        public:

            D3D12Texture(ID3D12Device* pDevice, const Image& image);                                //  
            D3D12Texture(ID3D12Device* pDevice, const Image& image, const vector<Image>& mipmaps);   //
            D3D12Texture(ID3D12Device* pDevice, StringView path);   //

            IntVec2 size()const noexcept;

            //TextureFormat getFormat()const;

            bool isMipped()const noexcept;

            bool isSRGB()const noexcept;

            bool hasDepth()const noexcept;

            //-------------------
            ID3D12DescriptorHeap* getSRVPtr();


        private:

            ComPtr<ID3D12Resource>  m_resource;     //!< D3D12のデバイス本体
            vector<Texture> m_mipmaps;              //!< ミップマップ


        };



    }// namespace graphic
}// namespace ob