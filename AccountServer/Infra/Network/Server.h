#pragma once
#include <string_view>
#include <memory>
#include "./Connection.h"
#include <asio.hpp>
#include <vector>

namespace Eternal
{
	namespace Infra
	{
		using asio::ip::tcp;

		class Server
		{
		private:
			void init(std::string_view ip, uint16_t port);
			void on_accept(const asio::error_code& error, tcp::socket perr);
			void on_receive(std::shared_ptr<Connection> connection, size_t bytes_received);
			
		public:
			Server(std::string_view config_file);
			Server(std::string_view ip, uint16_t port);
			Server(const Server&) = delete;
			Server& operator=(const Server&) = delete;
			Server(Server&&) = default;
			~Server() = default;

		public:
			void set_iocontext(std::shared_ptr<asio::io_context> io_context) { _io_context = io_context; }
			void take_over();
			Server& set_threads(uint32_t thread_count);
			void shutdown();

		public:
			void run();

		public:
			std::function<void(uint8_t*, size_t)> _on_receive;

		private:
			std::shared_ptr<asio::io_context> _io_context;
			tcp::acceptor _acceptor;
			tcp::endpoint _endpoint;
			std::vector<std::shared_ptr<Connection>> _connections;
			std::vector<std::thread> _thread_pool;
		};
	}

}