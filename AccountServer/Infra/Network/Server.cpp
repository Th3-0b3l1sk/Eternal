#include "Server.h"
#include <iostream>
#include "./Connection.h"
#include "./Encryption/TqCipher.h"
#include "./Encryption/Rc5.h"
#include "../../Msg/MsgAccount.h"


namespace Eternal::Infra
{
	using asio::ip::tcp;

	void Server::init(std::string_view ip, uint16_t port)
	{
		_endpoint.address(asio::ip::make_address(ip));
		_endpoint.port(port);
		_acceptor.open(_endpoint.protocol());
		_acceptor.set_option(tcp::acceptor::keep_alive{ false });
		_acceptor.bind(_endpoint);
		_acceptor.listen();
	}

	void Server::on_accept(const asio::error_code& error, tcp::socket peer)
	{
		// todo: logger
		std::cout << " Accepeted connection from: " << peer.remote_endpoint().address().to_string() << " port: " << peer.remote_endpoint().port() << '\n';

		std::shared_ptr<Connection> client = std::make_shared<Connection>( std::move(peer), &Server::on_receive );
		_connections.push_back(std::move(client));
		_connections.back()->begin_read();    // TODO: replace the vector with map<id, connection>

		_acceptor.async_accept(std::bind(&Server::on_accept, this, std::placeholders::_1, std::placeholders::_2));
	}

	void Server::on_receive(std::shared_ptr<Connection> connection, size_t bytes_received)
	{
		// TODO: make the function more generic to run as an account server and the game server(server_name: string, fn_user_on_receive)
		if (bytes_received > 0) {
			std::cout << "Incoming [" << bytes_received << "] bytes from Client: " << connection->get_ip_address() << ":" << connection->get_port() << '\n';
			
			Encryption::TqCipher cipher;
			cipher.generate_iv();
			cipher.decrypt((uint8_t*)connection->get_buffer().get(), bytes_received);

			Msg::MsgAccount msg_account{ connection->get_buffer().get(), bytes_received};
			Encryption::Rc5 rc5;
			rc5.decrypt((uint8_t*)msg_account.get_password(), msg_account.get_password_len());
			
			std::cout << msg_account.stringfy() << '\n';

			std::cout << "Resetting the connection...\n";
			connection->reset();
		}
	}

	void Server::run()
	{
		// todo: logger
		std::cout << "AccountServer is up and running ..." << '\n';
		_acceptor.async_accept(std::bind(&Server::on_accept, this,  std::placeholders::_1, std::placeholders::_2));

		auto work_guard = asio::make_work_guard<asio::io_context>(*_io_context);
		_io_context->run();
	}



	/************************************************************
	* Constructors
	************************************************************/

	Server::Server(std::string_view ip, uint16_t port)
		: _io_context{ std::make_shared<asio::io_context>() }, _acceptor{*_io_context}
	{
		init(ip, port);
	}
	
}
