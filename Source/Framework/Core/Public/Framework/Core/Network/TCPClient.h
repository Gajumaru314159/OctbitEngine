//***********************************************************
/// @file
/// @brief  TCPクライアント
/// @author Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/CorePrivate.h>
#include <Framework/Core/Network/IPAddress.h>

namespace ob::core {

	/// TCPクライアント
	class TCPClient {
	public:

		/// コンストラクタ
		TCPClient();

		/// デストラクタ
		~TCPClient();

		/// 接続を確立する
		bool connect(IPAddress ip, u16 port);

		/// データを送信する
		bool send(char* data, size_t size);

		/// データを受信する
		/// @return 受信したデータのサイズ
		size_t receive(char* data, size_t size);

		/// 接続を閉じる
		void disconnect();

		/// 接続中かどうか
		bool isConnected() const;

	private:

		friend class TCPServer;

		/// TCPServerからの接続
		bool connect(IPAddress ip, u16 port,s32 socket);

	private:
		IPAddress m_ip;
		u16 m_port;
		s32 m_socket;
	};


}