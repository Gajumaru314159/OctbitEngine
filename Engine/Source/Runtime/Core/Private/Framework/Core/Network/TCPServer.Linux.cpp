//***********************************************************
/// @file
/// @brief  コア
/// @author Gajumaru
//***********************************************************
#include <Framework/Core/Network/TCPServer.h>
#include <Framework/Core/String/StringEncoder.h>

#ifdef OS_LINUX
#include <arpa/inet.h>
#include <cerrno>
#include <cstring>
#include <memory>
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

	TCPServer::TCPServer() {
		m_port = 0;
		m_socket = kInvalidSocket;
	}

	TCPServer::~TCPServer() {
		close();
	}

	bool TCPServer::open(u16 port) {
		close();

		int socket = ::socket(AF_INET, SOCK_STREAM, 0);
		if (socket == -1) {
			LOG_ERROR("[TCPServer] {}", GetLastSocketErrorMessage());
			return false;
		}

		int reuseFlag = 1;
		if (::setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, &reuseFlag, sizeof(reuseFlag)) == -1) {
			LOG_ERROR("[TCPServer] {}", GetLastSocketErrorMessage());
			::close(socket);
			return false;
		}

		sockaddr_in addr{};
		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr = htonl(INADDR_ANY);
		addr.sin_port = htons(port);

		if (::bind(socket, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) == -1) {
			LOG_ERROR("[TCPServer] {}", GetLastSocketErrorMessage());
			::close(socket);
			return false;
		}

		if (::listen(socket, SOMAXCONN) == -1) {
			LOG_ERROR("[TCPServer] {}", GetLastSocketErrorMessage());
			::close(socket);
			return false;
		}

		m_socket = FromNativeSocket(socket);
		m_port = static_cast<s16>(port);
		return true;
	}

	void TCPServer::close() {
		int socket = ToNativeSocket(m_socket);
		if (socket != -1) {
			::close(socket);
			m_socket = kInvalidSocket;
		}
	}

	auto TCPServer::accept() const -> UPtr<TCPClient> {
		int socket = ToNativeSocket(m_socket);
		if (socket == -1) {
			return nullptr;
		}

		sockaddr_in clientAddr{};
		socklen_t addrLen = sizeof(clientAddr);
		int clientSocket = ::accept(socket, reinterpret_cast<sockaddr*>(&clientAddr), &addrLen);
		if (clientSocket == -1) {
			if (errno != EAGAIN && errno != EWOULDBLOCK) {
				LOG_WARNING("[TCPServer] accept failed: {}", GetLastSocketErrorMessage());
			}
			return nullptr;
		}

		IPAddress ip(ntohl(clientAddr.sin_addr.s_addr));
		u16 port = ntohs(clientAddr.sin_port);

		auto client = std::make_unique<TCPClient>();
		if (!client->connect(ip, port, static_cast<u64>(clientSocket))) {
			::close(clientSocket);
			return nullptr;
		}

		return client;
	}

	bool TCPServer::isClosed() const {
		return m_socket == kInvalidSocket;
	}

}
#endif
