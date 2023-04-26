#pragma once
#include <asio.hpp>
#include <string_view>
#include <memory>
#include <unordered_map>
#include <vector>
#include "Msg/NetMsg.h"
#include "Database/Database.h"
#include "Connection.h"

namespace Eternal
{
	using asio::ip::tcp;

	class Server
		{
		public:
		enum class Which
		{
			ACCOUNT,
			GAME
		};

		public:
			static constexpr char SEAL[] = "TQServer";
			static constexpr uint8_t SEAL_LEN = UINT8_C(8);

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
			void take_over();
			Server& set_threads(uint32_t thread_count);
			void shutdown();
			void disconnect(uint32_t id);
			std::shared_ptr<Connection> get_connetion(uint32_t con_id) const { return _connections.at(con_id); }
			std::vector<std::unique_ptr<uint8_t[]>> execute_statement(std::unique_ptr<Database::IStatement>&& statement);
			void send(uint32_t con_id, std::shared_ptr<Eternal::Msg::NetMsg> msg);
			void send_n_kill(uint32_t con_id, std::shared_ptr<Eternal::Msg::NetMsg> msg, bool set_tq_server = true);
					

		public:
			void run();

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
		};
}
