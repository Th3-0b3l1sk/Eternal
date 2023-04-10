#include <iostream>
#include "Network/Server.h"
#include "Network/Connection.h"
#include "Network/Encryption/TqCipher.h"
#include "./Encryption/DiffieHellman.h"
#include "./Encryption/Blowfish.h"
#include "./Msg/MsgConnectEx.h"
#include "./Msg/MsgLoginProofA.h"
#include <string>
#include <thread>
#include <Windows.h>

int main()
{
	try {
		Eternal::Server GameServer("127.0.0.1", 5816);

		GameServer._on_accept = [&](std::shared_ptr<Eternal::Connection> connection) {
			// TODO: implement the Blowfish and DH exhange
			// MsgLoginProofA -> (BF & DH)
			auto bf = std::make_unique<Blowfish>("DR654dt34trg4UI6");
			connection->set_cipher(std::move(bf));

			const std::string P{ "E7A69EBDF105F2A6BBDEAD7E798F76A209AD73FB466431E2E7352ED262F8C558F10BEFEA977DE9E21DCEE9B04D245F300ECCBBA03E72630556D011023F9E857F" };
			const std::string G{ "05" };
			DiffieHellman dh(P, G);
			dh.generate_key();
			std::cout << dh.get_private_key() << '\n';
			std::cout << dh.get_public_key()  << " - " << dh.get_public_key().size() << '\n';

			auto msg = std::make_shared<MsgLoginProofA>( P, G, dh.get_public_key() );
			GameServer.queue_msg(msg);

			/*auto tq_cipher = std::make_unique<Eternal::Encryption::TqCipher>();
			tq_cipher->generate_iv();
			connection->set_cipher(std::move(tq_cipher));
			std::shared_ptr<Eternal::Msg::MsgConnectEx> test = std::make_shared<Eternal::Msg::MsgConnectEx>();
			test->set_authentication_code(10).set_client_identity(100).set_game_server_ip("192.192.192.1").set_game_server_port(0x1337);
			GameServer.queue_msg(test);*/
		};

		GameServer._on_receive = [&](std::shared_ptr<Eternal::Connection> connection, size_t bytes_received) {
			// TODO: might want to move it to connection.receive()
			auto data = connection->get_buffer().get();
			connection->get_cipher()->decrypt(data, bytes_received);
			auto msg = Eternal::Msg::NetMsg::create(connection->get_buffer(), bytes_received);
			msg->process(GameServer);
		};

		GameServer.take_over();
		//GameServer.set_threads(3).run();

		while (!(GetAsyncKeyState(VK_NUMPAD0) & 1))
		{
			Sleep(100);
		}

		GameServer.shutdown();
		std::cout << "Server stopped\n";
	}
	catch (std::exception& e)
	{
		std::cout << "Exception: " << e.what() << '\n';
	}

}