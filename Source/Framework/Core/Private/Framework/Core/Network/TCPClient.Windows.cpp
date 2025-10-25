//***********************************************************
/// @file
/// @brief  コア
/// @author Gajumaru
//***********************************************************
#include <Framework/Core/Network/TCPClient.h>
#include <Framework/Core/String/StringEncoder.h>

#ifdef OS_WINDOWS
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")

namespace ob::core {

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
	TCPClient::TCPClient() {
		m_ip = { 0,0,0,0 };
		m_port = 0;
		m_socket = INVALID_SOCKET;
	}

	/// デストラクタ
	TCPClient::~TCPClient() {
		disconnect();
	}

	/// 接続を確立する
	bool TCPClient::connect(IPAddress ip, u16 port) {

		struct sockaddr_in addr;
		addr.sin_family = AF_INET; // AF_INET==IPv4
		addr.sin_port = ::htons(port);
		addr.sin_addr.S_un.S_un_b.s_b1 = ip.a;
		addr.sin_addr.S_un.S_un_b.s_b2 = ip.b;
		addr.sin_addr.S_un.S_un_b.s_b3 = ip.c;
		addr.sin_addr.S_un.S_un_b.s_b4 = ip.d;

		auto socket = ::socket(addr.sin_family, SOCK_STREAM, IPPROTO_TCP);
		if (socket == INVALID_SOCKET) {
			LOG_ERROR("[TCPClient] {}", GetWSALastErrorMessage());
			return false;
		}

		auto error = ::connect(socket, reinterpret_cast<struct sockaddr*>(&addr), sizeof(addr));
		if (error != 0) {
			LOG_ERROR("[TCPClient] {}", GetWSALastErrorMessage());
			return false;
		}

		m_socket = (SOCKET)socket;
		m_ip = ip;
		m_port = port;
		return true;
	}

	/// TCPServerからの接続
	bool TCPClient::connect(IPAddress ip, u16 port, u64 socket) {
		m_socket = socket;
		m_ip = ip;
		m_port = port;
		return true;	
	}

	/// データを送信する
	bool TCPClient::send(const char* data, size_t size)
	{
		if (!isConnected()) {
			return false;
		}

		auto sentSize = ::send((SOCKET)m_socket, data, static_cast<int>(size), 0);
		if (sentSize == -1) {
			disconnect();
			return false;
		}

		return true;
	}

	/// データを受信する
	size_t TCPClient::receive(char* data, size_t size) {
		if (!isConnected()) {
			return 0;
		}

		auto receivedSize = ::recv((SOCKET)m_socket, data, static_cast<int>(size), 0);
		if (receivedSize == -1) {
			disconnect();
			return 0;
		}
		
		if (receivedSize == 0) {
			disconnect();
			return 0;
		}

		return receivedSize;
	}

	/// 接続を閉じる
	void TCPClient::disconnect() {
		if (isConnected()) {
			::closesocket(m_socket);
			m_socket = INVALID_SOCKET;
		}
	}

	/// 接続中かどうか
	bool TCPClient::isConnected() const {
		return m_socket != INVALID_SOCKET;
	}

}
#endif
