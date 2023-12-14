#include <iostream>
#include "Network/Server.h"
#include "Network/Connection.h"
#include "Database/Database.h"
#include "Util/IniFile.h"
#include "Entities/ItemManager.h"
#include "World.h"

namespace Eternal
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

		std::shared_ptr<Connection> client = std::make_shared<Connection>( std::move(peer), 
			std::bind(&Server::on_receive, this ,std::placeholders::_1, std::placeholders::_2), *this);

		auto unique_id = client->get_con_uid();
		_connections.insert({ unique_id, std::move(client) });

		_on_accept(_connections.at(unique_id));

		_connections[unique_id]->begin_read();
		_acceptor.async_accept(std::bind(&Server::on_accept, this, std::placeholders::_1, std::placeholders::_2));
	}

	void Server::on_receive(std::shared_ptr<Connection> connection, size_t bytes_received)
	{
		if (bytes_received > 0) {
			// TODO: proper logger
#ifdef _DEBUG
				std::cout << "Incoming [" << bytes_received << "] bytes from Client: " << connection->get_ip_address() << ":" << connection->get_port() << '\n';
#endif
			_on_receive(connection, bytes_received);
			if(connection->get_state() != Connection::State::CLOSED)
				connection->begin_read();
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
			connection.second->reset();
	}

	void Server::disconnect(uint32_t id)
	{
		// TODO: remove this logging
		std::cout << "Disconnecting client: " << id << '\n';
		_connections.at(id)->reset();
		_connections.erase(id);
	}

	std::vector<std::unique_ptr<uint8_t[]>>
		Server::execute_statement(std::unique_ptr<Database::IStatement>&& statement)
	{
		DebugBreak();
		// TODO: DB-FIX
		//return std::move(_database->execute(std::move(statement)));	
		return {};
	}

	static std::shared_ptr<uint8_t[]> prep_msg(std::shared_ptr<Eternal::Msg::NetMsg> msg, bool set_tq_server = true)
	{
		using Eternal::Server;
		std::shared_ptr<uint8_t[]> data(new uint8_t[msg->get_size() + (set_tq_server ? SEAL_LEN : 0)]{});
		memcpy_s(data.get(), msg->get_size(), msg->get_data().get(), msg->get_size());
		if (!set_tq_server)
			return std::move(data);

		memcpy_s(data.get() + msg->get_size(), SEAL_LEN, SEAL, SEAL_LEN);
		return std::move(data);
	}


	void Server::send(uint32_t con_id, std::shared_ptr<Eternal::Msg::NetMsg> msg)
	{
		auto packet = prep_msg(msg);
		_connections.at(con_id)->send(packet, msg->get_size() + SEAL_LEN);
		std::cout << msg->stringfy() << '\n';
	}

	void Server::send_n_kill(uint32_t con_id, std::shared_ptr<Eternal::Msg::NetMsg> msg, bool set_tq_server )
	{
		auto packet = prep_msg(msg, set_tq_server);
		std::cout << msg->stringfy() << '\n';
		_connections.at(con_id)->block();
		_connections.at(con_id)->write(packet, msg->get_size() + (set_tq_server ? SEAL_LEN : 0));
		disconnect(con_id);
	}

	std::unique_ptr<Eternal::World>& Server::get_world() 
	{ 
		return _game_world; 
	}

	void Server::set_world(std::unique_ptr<Eternal::World>&& world)
	{
		_game_world = std::move(world);
	}

	std::shared_ptr<Connection>& Server::get_connection(uint32_t con_id)
	{ 
		return _connections.at(con_id); 
	}

	std::unique_ptr<Database::Database>& Server::get_database()
	{
		return _database;
	}

	void Server::run()
	{
		// TODO: proper logger
		std::cout << "The server is up and running ..." << '\n';
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
	Server::Server(std::string_view ip, uint16_t port, std::unique_ptr<Database::Database>&& database)
		: _database(std::move(database)),
		_io_context{ std::make_shared<asio::io_context>() }, _acceptor{*_io_context}
	{
		init(ip, port);
	}

	Server::Server(std::string_view config_file)
		: _io_context{ std::make_shared<asio::io_context>() }, _acceptor{ *_io_context }

	{
		try {
			_config = std::make_unique<Util::IniFile>(config_file);
			auto& author = _config->get("server", "author");
			auto& ip = _config->get("server", "ip");
			auto port = _config->get<uint16_t>("server", "port");
			init(ip, port);

			auto& dsn = _config->get("database", "dsn");
			auto& usr = _config->get("database", "usr");
			auto& pwd = _config->get("database", "pwd");
			_database = std::make_unique<Database::Database>(dsn, usr, pwd);
		}
		catch (std::exception& e) {
			std::cerr << "[!] A fatal error has occured.\n\tError: " << e.what() << '\n';
			std::cerr << "\tShutting down the server!\n";
			::terminate();
		}
	}
}
