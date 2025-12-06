//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Reflection/Type.h>
#include <Framework/Core/Network/TCPClient.h>
#include <Framework/Core/Misc/Blob.h>

#include <Framework/Platform/Window.h>

namespace ob::editor {

	class Response {
	public:
		OB_RTTI();
		virtual ~Response() = default;
	protected:
		friend class Editor;
		virtual bool serialize(BinaryWriter& writer) const = 0;
	};

	class Query {
	public:
		OB_RTTI();

		virtual ~Query() = default;
	protected:
		friend class Editor;
		virtual bool deserialize(BinaryReader& reader) = 0;
		virtual Response* execute() const = 0;
	};

	class Notice {
	public:
		OB_RTTI();
		virtual ~Notice() = default;
	protected:
		friend class Editor;
		virtual bool serialize(BinaryWriter& writer) const = 0;
	};



	class LogNotice : public Notice {
	public:
		OB_RTTI();
		LogLevel level;
		String message;
	private:
		virtual bool serialize(BinaryWriter& writer) const {
			writer.writeUInt32(static_cast<u8>(level));
			writer.writeUInt64(message.size());
			writer.write(message.data(), message.size());
			return true;
		}
	};

	class AddViewportResponse : public Response {
	public:
		OB_RTTI();
	private:
		bool serialize(BinaryWriter& writer) const override {
			writer.writeUInt64(handle);
			return true;
		}
	public:
		u64 handle;
	};

	class AddViewportQuery : public Query {
	public:
		OB_RTTI();
	private:
		bool deserialize(BinaryReader& reader) override {

			return true;
		}
		Response* execute() const override {
			using namespace ob::platform;
			//WindowDesc desc;
			//desc.title = "Test";
			//Window window(desc);
			//s_window = window;
			// TODO ウィンドウ管理は誰がする？

			auto response = new AddViewportResponse();


			response->handle = reinterpret_cast<u64>(platform::Window::Main().getHandle());

			LOG_INFO("ハンドル {}", response->handle);
			//response->handle = reinterpret_cast<u64>(window.getHandle());

			return response;
		}
	};

	class CreateWorldResponse : public Response {
	public:
		OB_RTTI();
	private:
		bool serialize(BinaryWriter& writer) const override {
			writer.writeInt32(remoteId);
			return true;
		}
	public:
		s32 remoteId;
	};

	class CreateWorldQuery : public Query {
	public:
		OB_RTTI();
		String name;
	private:
		bool deserialize(BinaryReader& reader) override {
			//auto len = reader.readS32();
			//name.resize(len);
			//reader.read(name.data(), len);
			return true;
		}
		Response* execute() const override {
			auto response = new CreateWorldResponse();
			response->remoteId = 123;

			return response;
		}
	};


	//! @brief  エンジン
	class Editor {
	public:

		Editor();


		void send(const Notice& notice);


		void update() {
			flush();
		}
	private:
		void send(BlobView blob) {
			ScopeLock lock(m_sendBufferLock);
			m_sendBuffer.append(blob.data(), blob.size());
		}

		void flush();
		void receive();
	private:

		TCPClient	m_client;
		Blob		m_blob;
		Blob		m_writeBlob;

		SpinLock	m_sendBufferLock;
		Blob		m_sendBuffer;

		HashMap<u64, UPtr<Query>> m_queries;

		UPtr<Thread> m_thread;
	};

}