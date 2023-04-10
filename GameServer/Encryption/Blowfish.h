#pragma once
#include <string_view>
#include <openssl/blowfish.h>
#include <memory>

class Blowfish
{
private:
    enum class Do
    {
        Encrypt,
        Decrypt
    };
public:
    Blowfish(std::string_view key);

public:
    void encrypt(std::shared_ptr<uint8_t[]> ct, size_t len);
    void decrypt(std::shared_ptr<uint8_t[]> pt, size_t len);

private:
    void bf_do(std::shared_ptr<uint8_t[]> in_out, size_t len, Do what);

private:    
    std::unique_ptr<BF_KEY> _bf_key;
    std::unique_ptr<uint8_t[]> _en_iv;
    std::unique_ptr<uint8_t[]> _de_iv;
    uint32_t _en_num;
    uint32_t _de_num;
};

