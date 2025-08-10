//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#include <Plugins/DirectX12RHI/Descriptor/DirectX12DescriptorUploader.h>

namespace ob::rhi
{

	//! @brief  コンストラクタ
	DirectX12DescriptorUploader::DirectX12DescriptorUploader(ID3D12Device& device)
		: m_device(device)
	{
		m_frames.resize(2);
	}

	void DirectX12DescriptorUploader::add(D3D12_DESCRIPTOR_HEAP_TYPE type, D3D12_CPU_DESCRIPTOR_HANDLE src, D3D12_CPU_DESCRIPTOR_HANDLE dst) {

		ScopeLock lock(m_lock);

		auto& frame = m_frames.current();

		auto& request = frame.requests[type].emplace_back();
		request.src = src;
		request.dst = dst;

	}

	//! @brief 更新
	//! 
	//! この関数は1フレームに1回だけ呼び出す必要があります。
	void DirectX12DescriptorUploader::update() {

		auto& frame = m_frames.current();
		{
			ScopeLock lock(m_lock);
			m_frames.next();
		}

		for (s32 i = 0; i < std::size(frame.requests); ++i) {
			D3D12_DESCRIPTOR_HEAP_TYPE type = static_cast<D3D12_DESCRIPTOR_HEAP_TYPE>(i);
			for(auto& request : frame.requests[i]) {
				m_device.CopyDescriptorsSimple(1, request.dst, request.src, type);
			}
		}

		frame.clear();

	}

}