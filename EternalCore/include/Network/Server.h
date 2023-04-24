#pragma once
#include <string_view>
#include <memory>
#include "./Connection.h"
#include "../Msg/NetMsg.h"
#include "../Database/Database.h"
#include <asio.hpp>
#include <unordered_map>
#include <deque>

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

		private:
			void init(std::string_view ip, uint16_t port);
			void on_accept(const asio::error_code& error, tcp::socket perr);
			void on_receive(std::shared_ptr<Connection> connection, size_t bytes_received);
			
		public:
			Server(std::string_view config_file);
			Server(std::string_view ip, uint16_t port, std::unique_ptr<Database>&& database);
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

		private:
			void send(std::shared_ptr<Connection> connection, std::shared_ptr<Eternal::Msg::NetMsg> msg);

		public:
			void queue_msg(std::shared_ptr<Eternal::Msg::NetMsg> msg);

		public:
			void run();

		public:
			std::function<void(std::shared_ptr<Connection>, size_t)> _on_receive;
			std::function<void(std::shared_ptr<Connection>)> _on_accept;
			Which _which;
			// TODO: a multi-threaded nightmare, fixt it
			bool _disconnect_last = false;

		private:
			std::shared_ptr<asio::io_context> _io_context;
			tcp::acceptor _acceptor;
			tcp::endpoint _endpoint;
			std::unordered_map<uint32_t, std::shared_ptr<Connection>> _connections;
			std::deque<std::shared_ptr<Eternal::Msg::NetMsg>> _outgoing;
			std::vector<std::thread> _thread_pool;
			std::unique_ptr<Database> _database;
		};

}