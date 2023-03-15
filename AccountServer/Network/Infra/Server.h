#pragma once
#include <string_view>
#include <memory>
#include <asio.hpp>

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
			
		public:
			Server(std::string_view config_file);
			Server(std::string_view ip, uint16_t port);
			Server(const Server&) = delete;
			Server& operator=(const Server&) = delete;
			Server(Server&&) = default;
			~Server() = default;

		public:
			void set_iocontext(std::shared_ptr<asio::io_context> io_context) { _io_context = io_context; }
			std::shared_ptr<asio::io_context> get_iocontext() const { return _io_context; }

		public:
			void run();

		private:
			std::shared_ptr<asio::io_context> _io_context;
			tcp::acceptor _acceptor;
			tcp::endpoint _endpoint;
			
		};
	}

}