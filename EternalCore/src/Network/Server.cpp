#include <iostream>
#include "Network/Server.h"
#include "Network/Connection.h"
#include "Database/Database.h"
#include "Util/IniFile.h"
#include "Util/Logger.h"
#include "Entities/ItemManager.h"
#include "World.h"

// Global logger
extern std::unique_ptr<Eternal::Util::Logger> GServerLogger;

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
		std::string con = "Accepeted connection from: "+ peer.remote_endpoint().address().to_string() + " port: " + std::to_string(peer.remote_endpoint().port()) + '\n';
		Info(GServerLogger, con);

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
	
			std::string msg = "Incoming [" +  std::to_string(bytes_received) + "] bytes from Client: " + connection->get_ip_address() + ":" + std::to_string(connection->get_port()) + '\n';
			Verbose(GServerLogger, msg);
			
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
		Info(GServerLogger, "Disconnecting client: " + std::to_string(id) + '\n');

		_connections.at(id)->reset();
		_connections.erase(id);
	}

	std::vector<std::unique_ptr<uint8_t[]>>
		Server::execute_statement(std::unique_ptr<Database::IStatement>&& statement)
	{
		DebugBreak();
		// TODO: DB-FIX
		return {};
	}

	static std::shared_ptr<uint8_t[]> prep_msg(std::shared_ptr<Eternal::Msg::NetMsg> msg, bool set_tq_server = true)
	{
		using Eternal::Server;
		std::shared_ptr<uint8_t[]> data(new uint8_t[msg->get_size() + (set_tq_server ? SEAL_LEN : 0)]{});
		memcpy_s(data.get(), msg->get_size(), msg->get_data().get(), msg->get_size());
		if (!set_tq_server)
			return data;

		memcpy_s(data.get() + msg->get_size(), SEAL_LEN, SEAL, SEAL_LEN);
		return data;
	}


	void Server::send(uint32_t con_id, std::shared_ptr<Eternal::Msg::NetMsg> msg)
	{
		auto packet = prep_msg(msg);
		_connections.at(con_id)->send(packet, msg->get_size() + SEAL_LEN);
		
		Verbose(GServerLogger, "Sending to client [ " + std::to_string(con_id) + "] \n" + msg->stringfy() + '\n');
	}

	void Server::send_n_kill(uint32_t con_id, std::shared_ptr<Eternal::Msg::NetMsg> msg, bool set_tq_server )
	{
		auto packet = prep_msg(msg, set_tq_server);
		
		Verbose(GServerLogger, "SenKilling client [ " + std::to_string(con_id) + "] \n" + msg->stringfy() + '\n');

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
		Info(GServerLogger, "The Game server is up and running.");

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
			Fatal(GServerLogger, "[!] A fatal error has occured.\n\tError: " + std::string(e.what()) + "\nShutting down the server!");
			::terminate();
		}
	}
}
