#pragma once
#include <memory>
#include <string>
#include "Encryption/ICipher.h"
#include "Encryption/IExchange.h"
#include <asio.hpp>

namespace Eternal
{
    using asio::ip::tcp;

    namespace Entities
    {
        class Player;
    };

    namespace Msg
    {
        class NetMsg;
    };

    
    class Server;
    class Connection: public std::enable_shared_from_this<Connection>
        {
        static uint32_t id;

        public:
            enum class State
            {
                CLOSED,
                KEY_EXCHANGE,
                NORMAL
            };

        public:
            using fn_on_receive_callback = std::function<void(std::shared_ptr<Connection>, size_t bytes_received)>;

        public:
            // the callback signature: void(this)
            Connection(tcp::socket&& client_socket, fn_on_receive_callback on_receive_callback, Server& server);

        public:
            void begin_read();
            void reset();
            void block();
            void send(std::shared_ptr<uint8_t[]> data, size_t len);
            void write(std::shared_ptr<uint8_t[]> data, size_t len);
            void set_cipher(std::unique_ptr<Encryption::ICipher>&& cipher) { _cipher = std::move(cipher); }
            void set_state(State state) { _state = state; }
            void set_exchange(std::unique_ptr<Encryption::IExchange>&& dh) { _exchange = std::move(dh); }
            void set_player(std::shared_ptr<Entities::Player> player) { _player = std::move(player); }

        public:
            std::string get_ip_address() const { return _client_socket.remote_endpoint().address().to_string(); }
            uint16_t  get_port() const { return _client_socket.remote_endpoint().port(); }
            std::shared_ptr<uint8_t[]> get_buffer() const { return _buffer; }
            std::unique_ptr<Encryption::ICipher>& get_cipher() { return _cipher; }
            State get_state() const { return _state; }
            std::shared_ptr<Entities::Player>& get_player() { return _player; }
            std::unique_ptr<Encryption::IExchange>& get_exchange()  { return _exchange; }
            uint32_t get_player_id() const;
            uint32_t get_con_uid() const { return _unique_id; }
            void send_over_server(std::shared_ptr<Msg::NetMsg> msg);

        private:
            void on_receive(const asio::error_code& error, size_t bytes_read);

        private:
            Server& _server;
            tcp::socket _client_socket;
            fn_on_receive_callback _on_receive_callback;
            size_t BUF_SIZE = 1024;
            std::shared_ptr<uint8_t[]> _buffer;
            std::unique_ptr<Encryption::ICipher> _cipher;
            std::unique_ptr<Encryption::IExchange> _exchange;
            std::shared_ptr<Entities::Player> _player;
            State _state;
            uint32_t _unique_id;
        };
}
