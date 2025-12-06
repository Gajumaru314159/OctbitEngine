//***********************************************************
/// @file
/// @brief  TCPクライアント
/// @author Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/CorePrivate.h>
#include <Framework/Core/Network/TCPClient.h>

namespace ob::core {

	/// TCPクライアント
	class TCPServer {
	public:

		/// コンストラクタ
		TCPServer();

		/// デストラクタ
		~TCPServer();

		/// サーバーを開始する
		bool open(u16 port);

		/// サーバーを停止する
		void close();

		/// クライアントからの接続を受け入れる
		/// @details
		/// 新規のクライアント接続があった場合は、そのクライアントと通信するためのTCPClientを返します。
		/// 新規のクライアント接続がなかった場合は空のUPtrを返します。
		auto accept() const -> UPtr<TCPClient>;

		/// クローズ状態か
		bool isClosed() const;

	private:
		s16 m_port;
		u64 m_socket;
	};


}