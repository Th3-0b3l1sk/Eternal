#pragma once
#include <asio.hpp>
#include <string_view>
#include <memory>
#include <unordered_map>
#include <vector>
#include "Msg/NetMsg.h"
#include "Map/MapManager.h"

namespace Eternal
{
	namespace Database
	{
		class Database;
		class IStatement;
	};

	namespace Util
	{
		class IniFile;
	};

	using asio::ip::tcp;
	class World;
	class Connection;
	class Server
	{
	friend class World;	// for the db
	public:
	enum class Which
	{
		ACCOUNT,
		GAME
	};

	private:
		void init(std::string_view ip, uint16_t port);
		void on_accept(const asio::error_code& error, tcp::socket perr);
		void on_receive(std::shared_ptr<Connection> connection, size_t bytes_received);
		
	public:
		Server(std::string_view config_file);
		Server(std::string_view ip, uint16_t port, std::unique_ptr<Database::Database>&& database);
		Server(const Server&) = delete;
		Server& operator=(const Server&) = delete;
		Server(Server&&) = default;
		~Server() = default;

	public:
		void set_iocontext(std::shared_ptr<asio::io_context> io_context) { _io_context = io_context; }
		void set_world(std::unique_ptr<Eternal::World>&& world);
		std::unique_ptr<Eternal::World>& get_world();
		void take_over();
		Server& set_threads(uint32_t thread_count);
		void shutdown();
		void disconnect(uint32_t id);
		std::shared_ptr<Connection>& get_connection(uint32_t con_id);
		std::unique_ptr<Database::Database>& get_database() ;
		std::vector<std::unique_ptr<uint8_t[]>> execute_statement(std::unique_ptr<Database::IStatement>&& statement);
		void send(uint32_t con_id, std::shared_ptr<Eternal::Msg::NetMsg> msg);
		void send_n_kill(uint32_t con_id, std::shared_ptr<Eternal::Msg::NetMsg> msg, bool set_tq_server = true);

	public:
		void run();
		std::unique_ptr<Util::IniFile>& get_config() { return _config; }

	public:
		std::function<void(std::shared_ptr<Connection>, size_t)> _on_receive;
		std::function<void(std::shared_ptr<Connection>)> _on_accept;
		Which _which;

	private:
		std::shared_ptr<asio::io_context> _io_context;
		tcp::acceptor _acceptor;
		tcp::endpoint _endpoint;
		std::unordered_map<uint32_t, std::shared_ptr<Connection>> _connections;
		std::vector<std::thread> _thread_pool;
		std::unique_ptr<Database::Database> _database;
		std::unique_ptr<Eternal::World> _game_world;	// To be used only in the game server
		std::unique_ptr<Util::IniFile> _config;
	};
}
