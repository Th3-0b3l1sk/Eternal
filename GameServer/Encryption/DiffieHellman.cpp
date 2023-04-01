#include "DiffieHellman.h"
#include "openssl/dh.h"
#include "openssl/err.h"
#include "openssl/param_build.h"
#include "openssl/core_names.h"
#include <iostream> // TODO: remove me

DiffieHellman::DiffieHellman(std::string_view p, std::string_view q)
{
    ERR_clear_error();
    _prime.reset(hex_2_bn(p));
    if (!_prime) {
        std::cout << "Failed to convert the hex prime to a bigint. error code: " << ERR_get_error() << "\n";
        return;
    }
    _generator.reset(hex_2_bn(q));
    if (!_generator) {
        std::cout << "Failed to convert the hex generator to a bigint. error code: " << ERR_get_error() << "\n";
        return;
    } 
}

void DiffieHellman::generate_key()
{
    ERR_clear_error();
    std::unique_ptr<EVP_PKEY, deleter<EVP_PKEY_free>> domain_key{};
    generate_domain_key(&domain_key);
    std::unique_ptr<EVP_PKEY_CTX, deleter<EVP_PKEY_CTX_free>> generator_ctx{ EVP_PKEY_CTX_new_from_pkey(nullptr, domain_key.get(), nullptr) };
    if (!generator_ctx) {
        std::cout << "Failed to get generator ctx. error code: " << ERR_get_error() << "\n";
        return;
    }

    if (EVP_PKEY_keygen_init(generator_ctx.get()) <= 0) {
        std::cout << "Failed to init keygen. error code: " << ERR_get_error() << "\n";
        return;
    }
    EVP_PKEY* key = nullptr;
    if (EVP_PKEY_keygen(generator_ctx.get(), &key) <= 0) {
        std::cout << "Failed to generate the key. error code: " << ERR_get_error() << "\n";
        return;
    }

    _pkey.reset(key);
}

std::string DiffieHellman::derive_shared_secret(std::string_view hex_peer_public)
{
    ERR_clear_error();
    std::unique_ptr<EVP_PKEY, deleter<EVP_PKEY_free>> peer_pkey;
    create_peer_pkey(hex_peer_public, &peer_pkey);
    if (!peer_pkey) {
        std::cout << "Failed to create the peer public key. error: " << ERR_get_error() << '\n';
        return {};
    }
        

    std::unique_ptr<EVP_PKEY_CTX, deleter<EVP_PKEY_CTX_free>> ctx{ EVP_PKEY_CTX_new(_pkey.get(), nullptr) };
    if (!ctx) {
        std::cout << "Failed to create the ctx. error: " << ERR_get_error() << '\n';
        return {};
    }
    
    if (EVP_PKEY_derive_init(ctx.get()) <= 0) {
        char errorString[256];
        unsigned long lastError = ERR_peek_last_error();
        ERR_error_string_n(lastError, errorString, sizeof(errorString));
        std::cout << "Failed to derive init. error: " << ERR_get_error() << '\n';
        std::cout << errorString << '\n';
        return {};
    }

    if (EVP_PKEY_derive_set_peer(ctx.get(), peer_pkey.get()) <= 0) {

        char errorString[256];
        unsigned long lastError = ERR_peek_last_error();
        ERR_error_string_n(lastError, errorString, sizeof(errorString));
        std::cout << "Failed to derive init. error: " << ERR_get_error() << '\n';
        std::cout << errorString << '\n';
        return {};
    }

    size_t len{};
    if (EVP_PKEY_derive(ctx.get(), nullptr, &len) <= 0) {
        char errorString[256];
        unsigned long lastError = ERR_peek_last_error();
        ERR_error_string_n(lastError, errorString, sizeof(errorString));
        std::cout << "Failed to derive init. error: " << ERR_get_error() << '\n';
        std::cout << errorString << '\n';
        return {};
    }
    if (len == 0) {
        char errorString[256];
        unsigned long lastError = ERR_peek_last_error();
        ERR_error_string_n(lastError, errorString, sizeof(errorString));
        std::cout << "Failed to derive init. error: " << ERR_get_error() << '\n';
        std::cout << errorString << '\n';
        return {};
    }

    std::string shared_sec{};
    shared_sec.resize(len);
    if (EVP_PKEY_derive(ctx.get(), (uint8_t*)&shared_sec.front(), &len) <= 0) {
        char errorString[256];
        unsigned long lastError = ERR_peek_last_error();
        ERR_error_string_n(lastError, errorString, sizeof(errorString));
        std::cout << "Failed to derive init. error: " << ERR_get_error() << '\n';
        std::cout << errorString << '\n';
        return {};
    }

    return std::move(shared_sec);
}

BIGNUM* DiffieHellman::hex_2_bn(std::string_view hx)
{
    BIGNUM* bn = NULL;
    if (!BN_hex2bn(&bn, hx.data()))
        return nullptr;
    
    return bn;

}

std::string DiffieHellman::bn_2_hex(BIGNUM* bn)
{
    return BN_bn2hex(bn);
}


void DiffieHellman::generate_domain_key(std::unique_ptr<EVP_PKEY, deleter<EVP_PKEY_free>>* pkey)
{
    ERR_clear_error();
    std::unique_ptr<OSSL_PARAM_BLD, deleter<OSSL_PARAM_BLD_free>> param_builder{ OSSL_PARAM_BLD_new() };
    if (!param_builder) {
        std::cout << "Failed to create parameter builder. error code: " << ERR_get_error() << '\n';
        return;
    }

    if (!OSSL_PARAM_BLD_push_BN(param_builder.get(), OSSL_PKEY_PARAM_FFC_P, _prime.get()) ||
        !OSSL_PARAM_BLD_push_BN(param_builder.get(), OSSL_PKEY_PARAM_FFC_G, _generator.get())) {
        std::cout << "Failed to set the prime or the generator. error code: " << ERR_get_error() << '\n';
        return;
    }

    std::unique_ptr<OSSL_PARAM, deleter<OSSL_PARAM_free>> parameter{ OSSL_PARAM_BLD_to_param(param_builder.get()) };
    if (!parameter) {
        std::cout << "Failed to get the parameter from bld_to_param. error code: " << ERR_get_error() << '\n';
        return;
    }

    std::unique_ptr<EVP_PKEY_CTX, deleter<EVP_PKEY_CTX_free>> pkey_ctx{ EVP_PKEY_CTX_new_from_name(nullptr, "DHX", nullptr) };
    if (!pkey_ctx) {
        std::cout << "Failed to create the pkey ctx. error code: " << ERR_get_error() << '\n';
        return;
    }

    if(EVP_PKEY_fromdata_init(pkey_ctx.get()) <= 0) {
        std::cout << "Failed to init the pkey ctx. error code: " << ERR_get_error() << '\n';
        return;
    }

    EVP_PKEY* tmp = nullptr;
    if (EVP_PKEY_fromdata(pkey_ctx.get(), &tmp, EVP_PKEY_KEY_PARAMETERS, parameter.get()) <= 0) {
        std::cout << "Failed to create the pkey. error code: " << ERR_get_error() << '\n';
        return;
    }

    pkey->reset(tmp);
}

void DiffieHellman::create_peer_pkey(std::string_view hex_peer_public, std::unique_ptr<EVP_PKEY, deleter<EVP_PKEY_free>>* pkey)
{
    std::unique_ptr<BIGNUM, deleter<BN_free>> bn_pkey{ hex_2_bn(hex_peer_public) };
    if (!bn_pkey) {
        std::cout << "Failed to convert the peer public key to bignum. error code: " << ERR_get_error() << '\n';
        return;
    }

    std::unique_ptr<OSSL_PARAM_BLD, deleter<OSSL_PARAM_BLD_free>> param_bld{ OSSL_PARAM_BLD_new() };
    if (!param_bld) {
        std::cout << "Failed to create param bld. error code: " << ERR_get_error() << '\n';
        return;
    }

    if (!OSSL_PARAM_BLD_push_BN(param_bld.get(), OSSL_PKEY_PARAM_PUB_KEY, bn_pkey.get())) {
        std::cout << "Failed to push the public key. error code: " << ERR_get_error() << '\n';
        return;
    }

    if(!OSSL_PARAM_BLD_push_BN(param_bld.get(), OSSL_PKEY_PARAM_FFC_P, _prime.get()) || 
        !OSSL_PARAM_BLD_push_BN(param_bld.get(), OSSL_PKEY_PARAM_FFC_G, _generator.get())) {
        std::cout << "Failed to push either p or g. error code: " << ERR_get_error() << '\n';
        return;
    }

    std::unique_ptr<OSSL_PARAM, deleter<OSSL_PARAM_free>> parameter{ OSSL_PARAM_BLD_to_param(param_bld.get()) };
    if (!parameter) {
        std::cout << "Failed to create the param. error: " << ERR_get_error() << '\n';
        return;
    }

    std::unique_ptr<EVP_PKEY_CTX, deleter<EVP_PKEY_CTX_free>> ctx{ EVP_PKEY_CTX_new_from_name(nullptr, "DHX", nullptr) };
    if (!ctx) {
        std::cout << "Failed to create the context. error: " << ERR_get_error() << '\n';
        return;
    }

    if (EVP_PKEY_fromdata_init(ctx.get()) <= 0) {
        std::cout << "Failed to from data. error: " << ERR_get_error() << '\n';
        return;
    }

    EVP_PKEY* tmp = nullptr;
    if(EVP_PKEY_fromdata(ctx.get(), &tmp, EVP_PKEY_PUBLIC_KEY, parameter.get()) <= 0) {
        std::cout << "Failed to from data 2. error: " << ERR_get_error() << '\n';
        return;
    }

    pkey->reset(tmp);
}

std::string DiffieHellman::get_private_key()
{
    ERR_clear_error();

    BIGNUM* pkey = nullptr;
    std::string key;
    if (!EVP_PKEY_get_bn_param(_pkey.get(), OSSL_PKEY_PARAM_PRIV_KEY, &pkey)) {
        std::cout << "Failed to get the private key. error code: " << ERR_get_error() << '\n';
        return {};
    }

    std::unique_ptr<BIGNUM, deleter<BN_free>> private_key_holder{ pkey};

    key = bn_2_hex(pkey);
    if (key.empty()) {
        std::cout << "Failed to get the private key. error code: " << ERR_get_error() << '\n';
        return {};
    }


    return key;
}

std::string DiffieHellman::get_public_key()
{
    ERR_clear_error();

    BIGNUM* pkey = nullptr;
    std::string key;
    if (!EVP_PKEY_get_bn_param(_pkey.get(), OSSL_PKEY_PARAM_PUB_KEY, &pkey)) {
        std::cout << "Failed to get the private key. error code: " << ERR_get_error() << '\n';
        return {};
    }

    std::unique_ptr<BIGNUM, deleter<BN_free>> public_key_holder{ pkey };

    key = bn_2_hex(pkey);
    if (key.empty()) {
        std::cout << "Failed to get the public key. error code: " << ERR_get_error() << '\n';
        return {};
    }


    return key;
}
