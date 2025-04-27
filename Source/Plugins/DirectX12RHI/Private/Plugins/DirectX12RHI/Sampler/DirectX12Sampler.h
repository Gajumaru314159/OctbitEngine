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
    class DirectX12RHI;
    class ITexture;
}


//===============================================================
// クラス定義
//===============================================================
namespace ob::rhi {

    //! @brief  サンプラー実装(DirectX12)
    class DirectX12Sampler :public Sampler {
    public:

        DirectX12Sampler(DirectX12RHI& rhi, const SamplerDesc& desc);
		D3D12_CPU_DESCRIPTOR_HANDLE getCopyableHandle() const { return m_handle.getCpuHandle(); }

        //! @brief  妥当な状態か
        bool isValid()const { return !m_handle.empty(); }


        //! @brief      名前を取得
        const String& getName()const override { return m_name; }

    private:
        DirectX12RHI& m_rhi;
        D3D12_SAMPLER_DESC m_desc;
        String m_name;
        DescriptorHandle m_handle;
    };

}