#include <Framework/Core/Utility/DI.h>
#include <Framework/Core/Network/Network.h>
#include <Framework/Core/Network/TCPServer.h>

//-----------------------------------------------------------------
using namespace ob::core;

int main() {
	Logger logger;

	Network::Startup();
	{
		Vector<UPtr<TCPClient>> clients;
		TCPServer server;

		server.open(5000);

		while (!server.isClosed()) {
			auto client = server.accept();
			if (client) {
				client->send("Hello", 6);
				clients.emplace_back(std::move(client));
			}
			for (auto& client : clients) {
				char data[1024]{};
				if (auto size = client->receive(data, sizeof(data))) {
					LOG_INFO("{}",data);
				}
			}
			Thread::Sleep(1000);
		}

	}
	Network::Shutdown();
	return 0;
}