//***********************************************************
//! @file
//! @author Gajumaru
//***********************************************************
#include <Framework/Core/Network/Network.h>

#ifdef OS_WINDOWS
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")
#else
#error "Unsupported platform"
#endif

namespace ob::core {


#ifdef OS_WINDOWS
	static bool s_isStartedUp = false;

	/// ネットワーク機能の開始
	void Network::Startup() {
		if (s_isStartedUp == false) {
			WSAData wsaData;
			int error = WSAStartup(MAKEWORD(2, 2), &wsaData);
			if (error != 0)
			{
				LOG_ERROR("[Socket] {}", error);
				return;
			}
			s_isStartedUp = true;
		}
		else {
			LOG_WARNING("[Socket] ソケットはスタートアップ済みです");
		}
	}

	/// ネットワーク機能の終了
	void Network::Shutdown() {
		if (s_isStartedUp == true) {
			WSACleanup();
			s_isStartedUp = false;
		}
		else {
			LOG_WARNING("[Socket] ソケットはシャットダウン済みです");
		}
	}
#endif

}