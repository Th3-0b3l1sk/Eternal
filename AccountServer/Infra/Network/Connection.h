#pragma once
#include <asio.hpp>
#include <memory>
#include <string>

namespace Eternal
{
    namespace Infra
    {
        using asio::ip::tcp;
       
        class Connection: public std::enable_shared_from_this<Connection>
        {
            class Server;
        public:
            using fn_on_receive_callback = std::function<void(std::shared_ptr<Connection>, size_t bytes_received)>;

        public:
            // the callback signature: void(this)
            Connection(tcp::socket&& client_socket, fn_on_receive_callback on_receive_callback);

        public:
            void begin_read();
            void reset();

        public:

            std::string get_ip_address() const { return _client_socket.remote_endpoint().address().to_string(); }
            uint16_t  get_port() const { return _client_socket.remote_endpoint().port(); }
            std::shared_ptr<uint8_t[]> get_buffer() const { return _buffer; }

        private:
            void on_receive(const asio::error_code& error, size_t bytes_read);

        private:
            tcp::socket _client_socket;
            fn_on_receive_callback _on_receive_callback;
            size_t BUF_SIZE = 1024;
            std::shared_ptr<uint8_t[]> _buffer;

        };

    }
}