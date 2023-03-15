#include "Connection.h"

namespace Eternal::Infra
{
    
    Connection::Connection(tcp::socket&& client_socket, fn_on_receive_callback on_receive_callback)
        : _client_socket(std::move(client_socket)),
        _on_receive_callback(on_receive_callback),
        _buffer(std::make_unique<uint8_t[]>(BUF_SIZE))
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
                    _on_receive_callback(shared_from_this(), bytes_read);
                    this->begin_read();
                }

            });
    }

    void Connection::reset()
    {
        _client_socket.shutdown(tcp::socket::shutdown_both);
        _client_socket.close();
    }
    
   

}
