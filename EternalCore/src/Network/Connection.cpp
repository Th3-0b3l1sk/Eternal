#include <iostream> // TODO: remove
#include "Network/Connection.h"

namespace Eternal
{
    uint32_t Connection::id = 0;

    Connection::Connection(tcp::socket&& client_socket, fn_on_receive_callback on_receive_callback)
        : _client_socket(std::move(client_socket)),
        _on_receive_callback(on_receive_callback),
        _buffer(new uint8_t[BUF_SIZE] {}),
        unique_id(++id)
    {
        if (_on_receive_callback == nullptr)
            throw std::exception{}; // todo: proper exception

    }

    void Connection::begin_read()
    {
        _client_socket.async_receive(asio::mutable_buffer(_buffer.get(), BUF_SIZE),
            [this](const asio::error_code& error, size_t bytes_read) {
                if (!error)
                {
                    try {
                        if (_state == State::CLOSED) 
                            return;
                        _on_receive_callback(shared_from_this(), bytes_read);
                    }
                    catch (std::exception& e) {
                        std::cout << "An exception has occured. error: " << e.what() << '\n';
                    }
                }

            });
    }

    void Connection::reset()
    {
        _state = State::CLOSED;
        _client_socket.shutdown(tcp::socket::shutdown_both);
        _client_socket.cancel();
        _client_socket.close();
    }

    void Connection::block()
    {
        _state = State::CLOSED;
        _client_socket.shutdown(tcp::socket::shutdown_receive);
        _client_socket.cancel();
    }

    void Connection::send(std::shared_ptr<uint8_t[]> data, size_t len)
    {
        _cipher->encrypt(data.get(), len);
        asio::async_write(_client_socket, asio::mutable_buffer{ data.get(), len }, [this](const asio::error_code& error, size_t bytes_sent) {
            // TODO: remove this cout dependency. used only for debugging
            if (!error) {
                std::cout << "sent [" << bytes_sent << "] bytes to the client " << get_ip_address() << "@" << get_port() << '\n';
            }
            }
        );
    }
    // synchronous operation
    void Connection::write(std::shared_ptr<uint8_t[]> data, size_t len)
    {
        _cipher->encrypt(data.get(), len);
        asio::write(_client_socket, asio::const_buffer{ data.get(), len });
    }

}
