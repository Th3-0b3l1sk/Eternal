#pragma once
#include <string>
#include "openssl/bn.h"
#include "openssl/evp.h"
#include <type_traits>
#include <memory>

class DiffieHellman
{
private:
    
    template<auto fn>
    using deleter = std::integral_constant<decltype(fn), fn>;

public:
    DiffieHellman(std::string_view p, std::string_view q);
    
public:
    void generate_key();
    std::string derive_shared_secret(std::string_view hex_peer_public);
    std::string get_private_key();
    std::string get_public_key();

    std::string bn_2_hex(BIGNUM* bn);

private:
    BIGNUM* hex_2_bn(std::string_view hx);
    //std::string bn_2_hex(BIGNUM* bn);
    void generate_domain_key(std::unique_ptr<EVP_PKEY, deleter<EVP_PKEY_free>>* pkey);
    void create_peer_pkey(std::string_view hex_peer_public, std::unique_ptr<EVP_PKEY, deleter<EVP_PKEY_free>>* pkey);
    

private:
    std::unique_ptr<BIGNUM, deleter<BN_free>> _prime;
    std::unique_ptr<BIGNUM, deleter<BN_free>> _generator;
    std::unique_ptr<EVP_PKEY, deleter<EVP_PKEY_free>> _pkey;
};

