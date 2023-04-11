#include "DiffieHellman.h"
#include "openssl/dh.h"
#include "openssl/err.h"
#include <iostream> // TODO: remove me

DiffieHellman::DiffieHellman(std::string_view p, std::string_view g)
    : _dh(DH_new())
{
    ERR_clear_error();
    BIGNUM* prime = nullptr;
    if (0 == BN_hex2bn(&prime, p.data()))
    {
        std::cout << "Failed to convert the prime number from hex to bn. error: " << ERR_get_error() << '\n';
    }
    BIGNUM* generator = nullptr;
    if (0 == BN_hex2bn(&generator, g.data()))
    {
        std::cout << "Failed to convert the generator number from hex to bn. error: " << ERR_get_error() << '\n';
    }

    if (1 != DH_set0_pqg(_dh.get(), prime, NULL, generator))
    {
        std::cout << "Failed to convert the prime or the generator. error: " << ERR_get_error() << '\n';
    }
}

void DiffieHellman::generate_key()
{
   ERR_clear_error();
   if (0 == DH_generate_key(_dh.get()))
   {
       std::cout << "Failed to generate the key pair. error: " << ERR_get_error() << '\n';
   }
}

std::string DiffieHellman::derive_shared_secret(std::string_view hex_peer_public)
{
    ERR_clear_error();
    BIGNUM* peer_bn = nullptr;
    if (0 == BN_hex2bn(&peer_bn, hex_peer_public.data()))
    {
        std::cout << "Failed to convert the peer prime number from hex to bn. error: " << ERR_get_error() << '\n';
        return {};
    }

    uint8_t* key = new uint8_t[DH_size(_dh.get())]{};
    if (0 == DH_compute_key(key, peer_bn, _dh.get()))
    {
        std::cout << "Failed to compute the shared key. error: " << ERR_get_error() << '\n';
        return {};
    }

    std::string shared_secret((char*)key, DH_size(_dh.get()));
    delete[] key;

    return shared_secret;
}

std::string DiffieHellman::get_private_key()
{
    ERR_clear_error();
    const BIGNUM* pkey = DH_get0_priv_key(_dh.get());
    auto hex = BN_bn2hex(pkey);
    std::string private_key(hex);
    OPENSSL_free(hex);

    return private_key;
}

std::string DiffieHellman::get_public_key()
{
    ERR_clear_error();
    const BIGNUM* pkey = DH_get0_pub_key(_dh.get());
    auto hex = BN_bn2hex(pkey);
    std::string public_key(hex);
    OPENSSL_free(hex);

    return public_key;
}
