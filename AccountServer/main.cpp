#include <iostream>
#include "Network/Server.h"
#include "Network/Connection.h"
#include "Network/Encryption/TqCipher.h"
#include "Database/Database.h"
#include "Util/IniFile.h"
#include <string>
#include <thread>
#include "Entities/ItemManager.h" // NOT USED. KEEP
#include "Entities/NpcManager.h" // NOT USED. KEEP
#include "Map/MapManager.h" // NOT USED. KEEP
#include "World.h"	// NOT USED. KEEP


int main()
{
	try {

		Eternal::Server AccountServer("./config.ini");
		AccountServer._which = Eternal::Server::Which::ACCOUNT;

		AccountServer._on_accept = [&](std::shared_ptr<Eternal::Connection> connection) {
			auto tq_cipher = std::make_unique<Eternal::Encryption::TqCipher>();
			tq_cipher->generate_iv();
			connection->set_cipher(std::move(tq_cipher));
			};

		AccountServer._on_receive = [&](std::shared_ptr<Eternal::Connection> connection, size_t bytes_received) {
			// TODO: might want to move it to connection.receive()
			auto data = connection->get_buffer().get();
			connection->get_cipher()->decrypt(data, bytes_received);
			auto msg = Eternal::Msg::NetMsg::create(connection->get_buffer(), bytes_received);	
			msg->process(AccountServer, connection->get_con_uid());
		};

		AccountServer.take_over();
		while (!(GetAsyncKeyState(VK_NUMPAD0) & 1))
		{
			Sleep(100);
		}
		AccountServer.shutdown();
		std::cout << "Server stopped\n";
	}
	catch (std::exception& e)
	{
		std::cout << "Exception: " << e.what() << '\n';
	}
}
