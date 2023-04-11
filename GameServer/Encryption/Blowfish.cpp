#include <assert.h>
#include "Blowfish.h"
#include <openssl/err.h>
#include <memory>
#include <string>
#include <iostream>

Blowfish::Blowfish(std::string_view key)
    :_bf_key(nullptr),
    _en_iv(nullptr), _de_iv(nullptr)
{
    _cipher = Cipher::BF;
    _en_iv = std::make_unique<uint8_t[]>(BF_BLOCK);
    _de_iv = std::make_unique<uint8_t[]>(BF_BLOCK);
    _bf_key = std::make_unique<BF_KEY>();
    BF_set_key(_bf_key.get(), key.size(), (const uint8_t*)key.data());
}

void Blowfish::encrypt(uint8_t* ct, size_t len)
{
    bf_do(ct, len, Do::Encrypt);
}

void Blowfish::decrypt(uint8_t* pt, size_t len)
{  
    bf_do(pt, len, Do::Decrypt);
}

void Blowfish::bf_do(uint8_t* in_out, size_t len, Do what)
{
    assert(_bf_key != nullptr && "Invalid BF key.");
    switch (what)
    {
    case Blowfish::Do::Encrypt:
        BF_cfb64_encrypt(in_out, in_out, len, _bf_key.get(), _en_iv.get(), (int*)&_en_num, BF_ENCRYPT);
        break;
    case Blowfish::Do::Decrypt:
        BF_cfb64_encrypt(in_out, in_out, len, _bf_key.get(), _de_iv.get(), (int*)&_de_num, BF_DECRYPT);
        break;
    default:
        assert(0 && "Invalid code path.");
    }
}



