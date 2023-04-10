#pragma once
#include <string>
#include "openssl/bn.h"
#include <openssl/dh.h>
#include <memory>
#include <type_traits>

class DiffieHellman
{
private:
    template<auto Deleter>
    using deleter = std::integral_constant<decltype(Deleter), Deleter>;

public:
    DiffieHellman(std::string_view p, std::string_view q);
    
public:
    void generate_key();
    std::string derive_shared_secret(std::string_view hex_peer_public);
    std::string get_private_key();
    std::string get_public_key();

private:
    std::unique_ptr<DH, deleter<DH_free>> _dh;
    
};

