//***********************************************************
//! @file
//! @author Gajumaru
//***********************************************************
#include <Framework/Core/Network/Network.h>

#ifdef OS_LINUX
#include <sys/types.h>
#include <sys/socket.h>

namespace ob::core {

	namespace {
		bool g_isStartedUp = false;
	}

	void Network::Startup() {
		if (!g_isStartedUp) {
			// Linux では明示的な初期化は不要だが、Windows と同様の使用パターンを検証するためフラグのみ保持する。
			g_isStartedUp = true;
		} else {
			LOG_WARNING("[Socket] ソケットはスタートアップ済みです");
		}
	}

	void Network::Shutdown() {
		if (g_isStartedUp) {
			g_isStartedUp = false;
		} else {
			LOG_WARNING("[Socket] ソケットはシャットダウン済みです");
		}
	}

}
#endif
