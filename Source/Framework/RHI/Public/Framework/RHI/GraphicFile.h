//***********************************************************
//! @file
//! @brief		非同期読み込みファイル
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/CorePrivate.h>
#include <Framework/Core/String/StringView.h>
#include <Framework/Core/Misc/BlobView.h>
#include <Framework/Core/Utility/MemoryStorage.h>
#include <Framework/Core/Utility/Singleton.h>
#include <Framework/RHI/Forward.h>

namespace ob::rhi {

	struct GraphicFileQueueDesc {
		s32 capacity = 0;
		String name;
	};

	//! @brief		非同期ファイル定義
	//! @details	* uncompressedSizeが0以外の場合はプラットフォームに応じた解凍処理が行われます。
	//!				* destが指定されている場合は指定先にファイルの内容が展開されます。
	//!				* destが指定されていない場合は内部的にバッファを確保し、そのバッファに展開されます。
	struct GraphicFileRequest {
		Ref<GraphicFileHandle> handle;			//! ファイルハンドル
		size_t			offset = 0;				//! 読み込み開始位置	
		size_t			size = 0;				//! 読み込みサイズ	
		size_t			uncompressedSize = 0;	//! 解凍後のサイズ
		
		struct BufferDesc {
			Ref<rhi::Buffer> buffer;
			size_t offset = 0;
		};
		struct TextureDesc {
			Ref<rhi::Texture> texture;
			s32 subresourceIndex = 0;
			u32 left = 0;
			u32 top = 0;
			u32 right = 0;
			u32 bottom = 0;
			u32 front = 0;
			u32 back = 0;
		};
		struct TextureSequenceDesc {
			Ref<Texture> texture;
			s32 firstSubresourceIndex = 0;
		};

		Variant<BufferDesc, TextureDesc, TextureSequenceDesc> dest;

	};

	//! @brief 非同期ファイルハンドル
	class GraphicFileHandle : public RefObject {
	public:
		static Ref<GraphicFileHandle> Create(StringView path);
	public:
		virtual const String& path()const = 0;
		virtual bool empty()const = 0;
		virtual size_t size()const = 0;
	};

	//! @brief 非同期ファイル読み込みイベント
	class GraphicFileEvent : public RefObject {
	public:
		static Ref<GraphicFileEvent> Create();
	public:
		virtual bool poll() const = 0;
		virtual void wait() const = 0;
	};

	//! @brief 非同期ファイル読み込みキュー
	//! @detail このクラスは非同期ファイル読み込みを管理するキューです。
	class GraphicFileQueue : public RefObject {
	public:
		static Ref<GraphicFileQueue> Create(const GraphicFileQueueDesc& desc = {});
	public:
		virtual void add(const GraphicFileRequest& desc) = 0;
		virtual void add(Ref<GraphicFileEvent>&) = 0;
		virtual void wait() = 0;
		virtual void submit() = 0;
		virtual void validate() = 0;
	};

	struct GraphicFileMipInfo {
		u32 offset = 0;
		u32 size = 0;
		u32 uncompressedSize = 0;
	};

	class GraphicFile {
	public:
		static bool Generate(StringView input, StringView output);
		static Vector<GraphicFileMipInfo> Prepare(StringView file);
	};

}