//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#include <Plugins/DirectX12RHI/Descriptor/DirectX12DescriptorLayout.h>

namespace ob::rhi
{

	//! @brief コンストラクタ
	DirectX12DescriptorLayout::DirectX12DescriptorLayout(const DescriptorLayoutDesc& desc)
		: m_desc(desc)
	{
		m_heapInfo.samplerNum = 0;
		m_heapInfo.othersNum = 0;

		for (auto& item : desc.items)
		{
			bool isSampler = item.type == BindingType::Sampler;
			auto& mapInfo = m_mapInfos.emplace_back();
			mapInfo.type = isSampler ? DescriptorHeapType::Sampler : DescriptorHeapType::CBV_SRV_UAV;
			mapInfo.index = isSampler ? m_heapInfo.samplerNum : m_heapInfo.othersNum;
			if (isSampler) m_heapInfo.samplerNum++;
			if (!isSampler) m_heapInfo.othersNum++;
		}

		manage();
	}

}