#include <iostream>
#include "Network/Server.h"
#include "Network/Connection.h"
#include "Network/Encryption/TqCipher.h"
#include "Database/Database.h"
#include <string>
#include <thread>
#include <Windows.h>

int main()
{
	try {
		auto account_db = std::make_unique<Eternal::Database::Database>("eternal_account", "e_account_db", "e_account_pd");
		account_db->load_statements("./account_stmts.txt");

		Eternal::Server AccountServer("127.0.0.1", 55099, std::move(account_db));
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
			msg->process(AccountServer);
			if (AccountServer._disconnect_last) {
				AccountServer._disconnect_last = false;	
				connection->reset();
				AccountServer.disconnect(connection->unique_id);
				std::cout << "Disconnecting client [" << connection->unique_id << "].\n";
			}
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