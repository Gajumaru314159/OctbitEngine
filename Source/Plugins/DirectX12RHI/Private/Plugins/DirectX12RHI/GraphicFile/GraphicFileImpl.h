//***********************************************************
//! @file
//! @brief		非同期読み込みファイル
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/GraphicFile.h>

namespace ob::rhi::dx12 {

	//! @brief 非同期ファイルハンドル
	class GraphicFileHandleImpl : public GraphicFileHandle {
	public:
		GraphicFileHandleImpl(IDStorageFactory& factory, StringView path);
		bool isValid()const;
		const String& path()const override;
		bool empty()const override;
		size_t size()const override;
		const ComPtr<IDStorageFile>& file()const;
	private:
		String m_path;
		ComPtr<IDStorageFile> m_file;
	};

	//! @brief 非同期ファイル読み込みイベント
	class GraphicFileEventImpl : public GraphicFileEvent {
	public:
		GraphicFileEventImpl();
		~GraphicFileEventImpl();
		bool isValid()const;
		void set(ComPtr<ID3D12Fence> fence,HANDLE handle);
		bool poll() const override;
		void wait() const override;
	private:
		HANDLE m_handle = nullptr;
		ComPtr<ID3D12Fence> m_fence;
	};

	//! @brief 非同期ファイル読み込みキュー
	//! @detail このクラスは非同期ファイル読み込みを管理するキューです。
	class GraphicFileQueueImpl : public GraphicFileQueue {
	public:
		GraphicFileQueueImpl(ID3D12Device8& device, IDStorageFactory& factory, const GraphicFileQueueDesc& desc);
		bool isValid()const;
		void add(const GraphicFileRequest& desc) override;
		void add(Ref<GraphicFileEvent>&) override;
		void wait() override;
		void submit() override;
		void validate() override;
	private:
		GraphicFileQueueDesc m_desc;
		ID3D12Device8& m_device;
		ComPtr<IDStorageQueue1> m_queue;
	};

	
	class GraphicFileImpl {
	public:
		static bool Generate(ID3D12Device8& device, StringView input, StringView output, s32 compressionLevel);
		static Vector<GraphicFileMipInfo> Prepare(StringView path);
	};

}