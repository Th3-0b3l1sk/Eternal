#include <iostream>
#include "./Eternal/Network/Server.h"
#include "./Eternal/Network/Connection.h"
#include "./Eternal/Network/Encryption/TqCipher.h"
#include <string>
#include <thread>
#include <Windows.h>

int main()
{
	try {
		Eternal::Server AccountServer("127.0.0.1", 55099);
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
		};

		AccountServer.take_over();
		//AccountServer.set_threads(3).run();

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