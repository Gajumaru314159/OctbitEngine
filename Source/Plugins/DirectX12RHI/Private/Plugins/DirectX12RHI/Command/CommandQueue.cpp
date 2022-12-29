//***********************************************************
//! @file
//! @brief		コマンドキュー
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Plugins/DirectX12RHI/Command/CommandQueue.h>
#include <Plugins/DirectX12RHI/Command/CommandListImpl.h>
#include <Plugins/DirectX12RHI/Device/DeviceImpl.h>
#include <Plugins/DirectX12RHI/Utility/Utility.h>

namespace ob::rhi::dx12 {


	//@―---------------------------------------------------------------------------
	//! @brief  コンストラクタ
	//@―---------------------------------------------------------------------------
	CommandQueue::CommandQueue(DeviceImpl& device) 
		:m_fenceVal(0)
	{
		HRESULT result;

		// コマンドキュー生成
		{
			D3D12_COMMAND_QUEUE_DESC desc{};
			desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
			desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

			result = device.getNative()->CreateCommandQueue(&desc, IID_PPV_ARGS(m_commandQueue.ReleaseAndGetAddressOf()));
			if (FAILED(result)) {
				Utility::OutputFatalLog(result, TC("ID3D12Device::CreatteCommandQueue()"));
				return;
			}
		}
		// フェンス生成
		{
			result = device.getNative()->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(m_fence.ReleaseAndGetAddressOf()));
			if (FAILED(result)) {
				Utility::OutputFatalLog(result, TC("ID3D12Device::CreateFence()"));
				return;
			}
		}

	}

	void CommandQueue::entryCommandList(const CommandList& commandList) {
		m_entriedCommandList.push_back(&commandList);
	}

	void CommandQueue::execute() {
		m_entriedNativeCommandList.clear();
		for (auto& cmdList : m_entriedCommandList) {
			m_entriedNativeCommandList.push_back(static_cast<const CommandListImpl*>(cmdList)->getNative());
		}
		m_entriedCommandList.clear();
		m_commandQueue->ExecuteCommandLists((UINT)m_entriedNativeCommandList.size(), m_entriedNativeCommandList.data());
	}

	void CommandQueue::wait() {
		// 描画待ち
		auto a = m_fence->GetCompletedValue();
		m_commandQueue->Signal(m_fence.Get(), ++m_fenceVal);
		auto b = m_fence->GetCompletedValue();
		if (m_fence->GetCompletedValue() < m_fenceVal)
		{
			auto event = CreateEvent(nullptr, false, false, nullptr);
			m_fence->SetEventOnCompletion(m_fenceVal, event);
			WaitForSingleObject(event, INFINITE);
			CloseHandle(event);
		}
	}

	void CommandQueue::setName(StringView name) {
		Utility::setName(m_commandQueue.Get(), name);

		String fenceName = Format(TC("{}_Fence"), name);
		Utility::setName(m_fence.Get(), fenceName);
	}

}// ob::rhi::dx12 