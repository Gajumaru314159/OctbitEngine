//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#include "DirectX12Sampler.h"
#include <Plugins/DirectX12RHI/DirectX12RHI.h>
#include <Plugins/DirectX12RHI/Utility/Utility.h>
#include <Plugins/DirectX12RHI/Utility/TypeConverter.h>

namespace ob::rhi {

    //! @brief				シェーダーコードからシェーダーオブジェクトを生成
    //!
    //! @param src			シェーダコード
    //! @param stage		シェーダステージ
    //! @param errorDest	エラー出力先文字列
    DirectX12Sampler::DirectX12Sampler(DirectX12RHI& rhi,const SamplerDesc& desc)
		: m_rhi(rhi)
		, m_desc(desc)
    {
		D3D12_SAMPLER_DESC ddesc = {};
        switch (desc.filter)
        {
        case TextureFillter::Point:
            ddesc.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
            break;
        case TextureFillter::Linear:
            ddesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
            break;
        default:
            break;
        }

        //ddesc.Filter = D3D12_DECODE_MIP_FILTER(TypeConverter::Convert(desc.filter));
        ddesc.AddressU = TypeConverter::Convert(desc.addressU);
        ddesc.AddressV = TypeConverter::Convert(desc.addressV);
        ddesc.AddressW = TypeConverter::Convert(desc.addressW);
        ddesc.MipLODBias = desc.mipLodBias;
        ddesc.MaxAnisotropy = TypeConverter::Convert(desc.anisotropy);
        ddesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
        ddesc.MinLOD = 0;
        ddesc.MaxLOD = 100; //  大きければよい

        m_nativeDesc = ddesc;

        m_rhi.allocateHandle(DescriptorHeapType::Sampler, m_handle, 1);

        createView(m_handle.getCpuHandle());

        manage();
    }

    void DirectX12Sampler::createView(D3D12_CPU_DESCRIPTOR_HANDLE handle) {        
        m_rhi.getNative()->CreateSampler(&m_nativeDesc, handle);
    }

}