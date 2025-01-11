//***********************************************************
/// @file
/// @brief  コア
/// @author Gajumaru
//***********************************************************
#include <Framework/Core/Network/TCPServer.h>
#include <Framework/Core/String/StringEncoder.h>

#ifdef OS_WINDOWS
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")
#else
#error "Unsupported platform"
#endif

namespace ob::core {

#ifdef OS_WINDOWS

	/// 最後のWSAエラーメッセージを取得する
	static String GetWSALastErrorMessage() {
		WCHAR buffer[256];
		DWORD code = ::WSAGetLastError();
		DWORD dwFlags = FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS;
		auto result = ::FormatMessageW(
			dwFlags,
			nullptr,
			code,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			buffer,
			(DWORD)std::size(buffer),
			nullptr
		);

		if (0 == result) {
			String message;
			StringEncoder::Encode(buffer, message);
			return "不明なエラー";
		}
		String message;
		StringEncoder::Encode(buffer, message);
		if (2 <= message.size())message.resize(message.size() - 2);
		String error = Format("{0}", message.c_str());

		return error;
	}

	/// コンストラクタ
	TCPServer::TCPServer() {
		m_port = 0;
		m_socket = INVALID_SOCKET;
	}

	/// デストラクタ
	TCPServer::~TCPServer() {
		close();
	}

	/// 接続を確立する
	bool TCPServer::open(u16 port) {

		// ソケットの作成
		m_socket = (s32)::socket(AF_INET, SOCK_STREAM, 0);
		if (m_socket == INVALID_SOCKET) {
			LOG_ERROR("[TCPServer] {}", GetWSALastErrorMessage());
			close();
			return false;
		}

		// ポート再利用の設定
		int reuseFlag = 1;
		if (::setsockopt(m_socket, SOL_SOCKET, SO_REUSEADDR, (const char*)&reuseFlag, sizeof(reuseFlag)) == -1) {
			LOG_ERROR("[TCPServer] {}", GetWSALastErrorMessage());
			close();
			return false;
		}

		// ソケットの設定
		sockaddr_in addr{};
		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr = htonl(INADDR_ANY);
		addr.sin_port = htons(port);
		if (::bind(m_socket, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
			LOG_ERROR("[TCPServer] {}", GetWSALastErrorMessage());
			close();
			return false;
		}

		// ソケットを接続可能状態にする
		if (::listen(m_socket, SOMAXCONN) == -1) {
			LOG_ERROR("[TCPServer] {}", GetWSALastErrorMessage());
			close();
			return false;
		}

		return true;
	}

	/// サーバーを停止する
	void TCPServer::close() {
		if (m_socket != INVALID_SOCKET) {
			::closesocket(m_socket);
			m_socket = INVALID_SOCKET;
		}
	}

	/// クライアントからの接続を受け入れる
	auto TCPServer::accept() -> UPtr<TCPClient> {
		sockaddr_in clientAddr;
		int addrLen = sizeof(clientAddr);
		SOCKET socket = ::accept(m_socket, reinterpret_cast<sockaddr*>(&clientAddr), &addrLen);
		if (socket == INVALID_SOCKET) {
			return nullptr;
		}

		IPAddress ip(
			(u8)clientAddr.sin_addr.S_un.S_un_b.s_b1,
			(u8)clientAddr.sin_addr.S_un.S_un_b.s_b2,
			(u8)clientAddr.sin_addr.S_un.S_un_b.s_b3,
			(u8)clientAddr.sin_addr.S_un.S_un_b.s_b4
		);
		u16 port = clientAddr.sin_port;
		
		auto client = std::make_unique<TCPClient>();
		if (!client->connect(ip, port, socket)) {
			client.reset();
		}

		return std::move(client);
	}

	/// クローズ状態か
	bool TCPServer::isClosed() const {
		return m_socket == INVALID_SOCKET;
	}

#endif

}