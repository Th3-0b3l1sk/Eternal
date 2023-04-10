#pragma once
#include <string_view>
#include <openssl/blowfish.h>
#include <memory>
#include "Network/Encryption/ICipher.h"

class Blowfish: public Eternal::Encryption::ICipher
{
private:
    enum class Do
    {
        Encrypt,
        Decrypt
    };
public:
    Blowfish(std::string_view key);
    virtual ~Blowfish() = default;

public:
    virtual void encrypt(uint8_t* ct, size_t len) override;
    virtual void decrypt(uint8_t* pt, size_t len) override;

private:
    void bf_do(uint8_t* in_out, size_t len, Do what);

private:    
    std::unique_ptr<BF_KEY> _bf_key;
    std::unique_ptr<uint8_t[]> _en_iv;
    std::unique_ptr<uint8_t[]> _de_iv;
    uint32_t _en_num;
    uint32_t _de_num;
};

