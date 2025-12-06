//***********************************************************
//! @file
//! @author Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/CorePrivate.h>
#include <Framework/Core/Network/IPAddress.h>

namespace ob::core {

	/// ネットワーク
	/// 
	/// ネットワーク機能を使用する場合は Startup() と Shutdown() をそれぞれ1回ずつ呼び出す必要があります。
	/// @code
	/// int main(){
	///		Network::Startup();
	///		{
	///			TCPClient client;
	///		}
	///		Network::Shutdown();
	/// }
	/// @endcode
	class Network {
	public:

		/// ネットワーク機能の開始
		static void Startup();

		/// ネットワーク機能の終了
		static void Shutdown();

	};

}