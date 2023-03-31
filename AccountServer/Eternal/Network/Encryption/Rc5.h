#pragma once
#include <cstdint>
#include <vector>
#include <memory>


namespace Eternal
{
    namespace Encryption
    {
        class Rc5
        {
        public:
            Rc5();
            ~Rc5() = default;

        private:
            void init_key_expansion();
            uint32_t rotl_32(uint32_t word, uint8_t n);
            uint32_t rotr_32(uint32_t word, uint8_t n);

        public:
            void decrypt(uint8_t* data, uint32_t len);
            void encrypt(uint8_t* data, uint32_t len);

        private:
            uint32_t _p;
            uint32_t _q;
            std::vector<uint32_t> _magics;
            std::unique_ptr<uint32_t[]> _key;

            const uint32_t ROUNDS = UINT32_C(12);
            const uint32_t KEY_SIZE = UINT32_C(120);  // in bytes
        };
    }
}


