//***********************************************************
/// @file
/// @brief  コア
/// @author Gajumaru
//***********************************************************
#include <Framework/Core/Network/TCPClient.h>
#include <Framework/Core/String/StringEncoder.h>

#ifdef OS_LINUX
#include <arpa/inet.h>
#include <cerrno>
#include <cstring>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

namespace ob::core {

	namespace {

		constexpr u64 kInvalidSocket = 0;

		inline u64 FromNativeSocket(int fd) {
			return fd >= 0 ? static_cast<u64>(fd) + 1 : kInvalidSocket;
		}

		inline int ToNativeSocket(u64 socket) {
			return socket == kInvalidSocket ? -1 : static_cast<int>(socket - 1);
		}

		static String GetLastSocketErrorMessage() {
			int err = errno;
			const char* message = std::strerror(err);
			if (message == nullptr) {
				return Format("不明なエラー (errno={0})", err);
			}
			return Format("{0} (errno={1})", message, err);
		}

	}

	TCPClient::TCPClient() {
		m_ip = { 0,0,0,0 };
		m_port = 0;
		m_socket = kInvalidSocket;
	}

	TCPClient::~TCPClient() {
		disconnect();
	}

	bool TCPClient::connect(IPAddress ip, u16 port) {
		int socket = ::socket(AF_INET, SOCK_STREAM, 0);
		if (socket == -1) {
			LOG_ERROR("[TCPClient] {}", GetLastSocketErrorMessage());
			return false;
		}

		sockaddr_in addr{};
		addr.sin_family = AF_INET;
		addr.sin_port = ::htons(port);
		addr.sin_addr.s_addr = ::htonl(
			(static_cast<u32>(ip.a) << 24) |
			(static_cast<u32>(ip.b) << 16) |
			(static_cast<u32>(ip.c) << 8) |
			(static_cast<u32>(ip.d))
		);

		if (::connect(socket, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) == -1) {
			LOG_ERROR("[TCPClient] {}", GetLastSocketErrorMessage());
			::close(socket);
			return false;
		}

		m_socket = FromNativeSocket(socket);
		m_ip = ip;
		m_port = port;
		return true;
	}

	bool TCPClient::connect(IPAddress ip, u16 port, u64 socket) {
		m_socket = FromNativeSocket(static_cast<int>(socket));
		m_ip = ip;
		m_port = port;
		return isConnected();
	}

	bool TCPClient::send(const char* data, size_t size) {
		int socket = ToNativeSocket(m_socket);
		if (socket == -1) {
			return false;
		}

#ifdef MSG_NOSIGNAL
		const int flags = MSG_NOSIGNAL;
#else
		const int flags = 0;
#endif

		size_t offset = 0;
		while (offset < size) {
			ssize_t sentSize = ::send(socket, data + offset, static_cast<int>(size - offset), flags);
			if (sentSize == -1) {
				if (errno == EINTR) {
					continue;
				}
				LOG_WARNING("[TCPClient] send failed: {}", GetLastSocketErrorMessage());
				disconnect();
				return false;
			}
			if (sentSize == 0) {
				LOG_WARNING("[TCPClient] send returned 0 bytes");
				disconnect();
				return false;
			}
			offset += static_cast<size_t>(sentSize);
		}

		return true;
	}

	size_t TCPClient::receive(char* data, size_t size) {
		int socket = ToNativeSocket(m_socket);
		if (socket == -1) {
			return 0;
		}

		while (true) {
			ssize_t receivedSize = ::recv(socket, data, static_cast<int>(size), 0);
			if (receivedSize == -1) {
				if (errno == EINTR) {
					continue;
				}
				LOG_WARNING("[TCPClient] recv failed: {}", GetLastSocketErrorMessage());
				disconnect();
				return 0;
			}

			if (receivedSize == 0) {
				disconnect();
				return 0;
			}

			return static_cast<size_t>(receivedSize);
		}
	}

	void TCPClient::disconnect() {
		int socket = ToNativeSocket(m_socket);
		if (socket != -1) {
			::close(socket);
			m_socket = kInvalidSocket;
		}
	}

	bool TCPClient::isConnected() const {
		return m_socket != kInvalidSocket;
	}

}
#endif
