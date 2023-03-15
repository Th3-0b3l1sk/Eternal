#include "Server.h"
#include <iostream>

namespace Eternal::Infra
{
	using asio::ip::tcp;

	void Server::init(std::string_view ip, uint16_t port)
	{
		tcp::acceptor::keep_alive keep_alive{ false };

		_endpoint.address(asio::ip::make_address(ip));
		_endpoint.port(port);
		_acceptor.open(_endpoint.protocol());
		_acceptor.set_option(keep_alive);
		_acceptor.bind(_endpoint);
		_acceptor.listen();

	}

	void Server::on_accept(const asio::error_code& error, tcp::socket peer)
	{
		// todo: logger
		std::cout << " Accepeted connection from: " << peer.remote_endpoint().address().to_string() << " port: " << peer.remote_endpoint().port() << '\n';
		peer.shutdown(tcp::socket::shutdown_both);
		peer.close();
		
		_acceptor.async_accept(std::bind(&Server::on_accept, this, std::placeholders::_1, std::placeholders::_2));
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
