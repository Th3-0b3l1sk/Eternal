#pragma once
#include <string>
#include "openssl/bn.h"
#include <openssl/dh.h>
#include <memory>
#include <type_traits>
#include "Network/Encryption/IExchange.h"

class DiffieHellman: public Eternal::Encryption::IExchange
{
private:
    template<auto Deleter>
    using deleter = std::integral_constant<decltype(Deleter), Deleter>;

public:
    DiffieHellman(std::string_view p, std::string_view q);
    virtual ~DiffieHellman() = default;
    
public:
    void generate_key() override;
    std::string derive_shared_secret(std::string_view hex_peer_public) override;
    std::string get_private_key() override;
    std::string get_public_key() override;

private:
    std::unique_ptr<DH, deleter<DH_free>> _dh;
    
};

