#include <iostream>
#include <string>
#include <thread>
#include "Network/Server.h"
#include "Network/Connection.h"
#include "Network/Encryption/TqCipher.h"
#include "Encryption/DiffieHellman.h"
#include "Encryption/Blowfish.h"
#include "Msg/MsgConnectEx.h"
#include "Msg/MsgLoginProofA.h"
#include "Network/Encryption/IExchange.h"
#include "Entities/ItemManager.h"
#include "Entities/NpcManager.h"
#include "Map/MapManager.h"
#include "Database/Database.h"
#include "Util/co_defs.h"
#include "Util/IniFile.h"
#include "World.h"


int main()
{
	try {
		
		Eternal::Server GameServer("./config.ini");
		auto game_world = std::make_unique<Eternal::World>(GameServer);
		GameServer._which = Eternal::Server::Which::GAME;
		GameServer.set_world(std::move(game_world));
		GameServer._on_accept = [&](std::shared_ptr<Eternal::Connection> connection) {

			auto bf = std::make_unique<Blowfish>("DR654dt34trg4UI6");
			connection->set_cipher(std::move(bf));
			const std::string P{ "E7A69EBDF105F2A6BBDEAD7E798F76A209AD73FB466431E2E7352ED262F8C558F10BEFEA977DE9E21DCEE9B04D245F300ECCBBA03E72630556D011023F9E857F" };
			const std::string G{ "05" };
			std::unique_ptr<Eternal::Encryption::IExchange> dh(new DiffieHellman(P, G));
			dh->generate_key();
			auto msg = std::make_shared<MsgLoginProofA>( P, G, dh->get_public_key() );
			GameServer.send(connection->get_con_uid(), msg);
			connection->set_exchange(std::move(dh));
			connection->set_state(Eternal::Connection::State::KEY_EXCHANGE);
		};

		GameServer._on_receive = [&](std::shared_ptr<Eternal::Connection> connection, size_t bytes_received) {
			switch (connection->get_state())
			{
			case Eternal::Connection::State::KEY_EXCHANGE:
			{
				auto& cipher = connection->get_cipher();
				auto buffer = connection->get_buffer();
				cipher->decrypt(buffer.get(), bytes_received);
				// parse the incoming response
				// TODO: Move the handling part to a different Object?
				uint8_t* ptr = buffer.get();
				ptr += UINT8_C(7);			// skip padding
				uint32_t incoming_size = *((uint32_t*)ptr);
				ptr += sizeof(uint32_t);	// skip incoming_size
				uint32_t junk_size = *((uint32_t*)ptr);
				ptr += junk_size ;
				ptr += sizeof(uint32_t);	// skip the junk_size
				uint32_t client_public_len = *((uint32_t*)ptr);
				ptr += sizeof(uint32_t);	// skip the client_public_len
				auto client_public = std::string_view((char*)ptr, client_public_len);
				auto& exchange = connection->get_exchange();
				auto shared_secret = exchange->derive_shared_secret(client_public);
				auto bf = std::make_unique<Blowfish>(shared_secret);
				connection->set_cipher(std::move(bf));
				connection->set_state(Eternal::Connection::State::NORMAL);
				break;
			}
			case Eternal::Connection::State::NORMAL:
			{
				auto data = connection->get_buffer().get();
				connection->get_cipher()->decrypt(data, bytes_received);
				auto size = *(uint16_t*)data + SEAL_LEN;
				// multiple packets in the same blob
				while (size <= bytes_received) {
					const auto packet_size = *(uint16_t*)data;
					auto packet = new uint8_t[packet_size];
					memcpy_s(packet, packet_size, data, packet_size);
					auto msg = Eternal::Msg::NetMsg::create(std::shared_ptr<uint8_t[]>(packet), packet_size);
					msg->process(GameServer, connection->get_con_uid());
					if (size >= bytes_received)
						break;
					data += size;
					size += *(uint16_t*)data + SEAL_LEN;
				}
				break;
			}
			}
		};

		GameServer.take_over();
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
