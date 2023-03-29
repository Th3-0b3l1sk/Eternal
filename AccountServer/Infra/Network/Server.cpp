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
		// TODO: proper logger
		std::cout << " Accepeted connection from: " << peer.remote_endpoint().address().to_string() << " port: " << peer.remote_endpoint().port() << '\n';

		std::shared_ptr<Connection> client = std::make_shared<Connection>( std::move(peer), std::bind(&Server::on_receive, this ,std::placeholders::_1, std::placeholders::_2));
		_connections.push_back(std::move(client));
		_connections.back()->begin_read();    // TODO: replace the vector with map<id, connection>

		_acceptor.async_accept(std::bind(&Server::on_accept, this, std::placeholders::_1, std::placeholders::_2));
	}

	void Server::on_receive(std::shared_ptr<Connection> connection, size_t bytes_received)
	{
		if (bytes_received > 0) {
			// TODO: proper logger
			std::cout << "Incoming [" << bytes_received << "] bytes from Client: " << connection->get_ip_address() << ":" << connection->get_port() << '\n';
			_on_receive(connection->get_buffer().get(), bytes_received);
			std::cout << "Resetting the connection ... \n";
			connection->reset();
		}
	}

	void Server::take_over()
	{
		this->run();
	}

	Server& Server::set_threads(uint32_t thread_count)
	{
		for (uint32_t i{}; i < thread_count; ++i)
			_thread_pool.push_back(std::thread{});

		return *this;
	}

	void Server::shutdown()
	{
		_io_context->stop();
		for (auto& connection : _connections)
			connection->reset();
	}

	void Server::run()
	{
		// TODO: proper logger
		std::cout << "AccountServer is up and running ..." << '\n';
		_acceptor.async_accept(std::bind(&Server::on_accept, this,  std::placeholders::_1, std::placeholders::_2));
		auto work_guard = asio::make_work_guard<asio::io_context>(*_io_context);
		
		// taking over the caller thread
		if (_thread_pool.empty()) {
			_io_context->run();
			return;
		}
		
		for (auto& thread : _thread_pool) {
			// std::bind() can't resolve overloaded functions 
			// ref: https://stackoverflow.com/questions/9048119/why-cant-stdbind-and-boostbind-be-used-interchangeably-in-this-boost-asio-t
			thread = std::thread{ std::bind(static_cast<asio::io_context::count_type(asio::io_service::*)()>(&asio::io_service::run), _io_context.get()) };
			thread.detach();
		}
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
