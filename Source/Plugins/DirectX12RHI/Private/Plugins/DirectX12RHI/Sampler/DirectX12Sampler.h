//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/Sampler.h>
#include <Plugins/DirectX12RHI/Descriptor/DescriptorHandle.h>

//===============================================================
// 前方宣言
//===============================================================
namespace ob::rhi {
    class DirectX12Device;
    class ITexture;
}


//===============================================================
// クラス定義
//===============================================================
namespace ob::rhi {

    //! @brief  サンプラー実装(DirectX12)
    class DirectX12Sampler :public Sampler {
    public:

        DirectX12Sampler(DirectX12Device& device, const SamplerDesc& desc);

        //! @brief  妥当な状態か
        bool isValid()const { return !m_handle.empty(); }


        //! @brief      名前を取得
        const String& getName()const override { return m_desc.name; }


        //! @brief      BindlessHandleを取得
        BindlessHandle getHandle()const override;

        void createView(D3D12_CPU_DESCRIPTOR_HANDLE handle);

    private:
        DirectX12Device&       m_device;
		SamplerDesc         m_desc;
        DescriptorHandle    m_handle;
        D3D12_SAMPLER_DESC  m_nativeDesc;
    };


    //!@ condn


    //! @brief      BindlessHandleを取得
    inline BindlessHandle DirectX12Sampler::getHandle()const {
        BindlessHandle handle;
        handle.type = BindingType::Sampler;
        handle.index = m_handle.getBindlessIndex();
        return handle;
    }


    //! @endcond

}