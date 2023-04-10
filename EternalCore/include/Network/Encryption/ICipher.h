#pragma once
namespace Eternal
{
    namespace Encryption
    {
        class ICipher
        {
        public:
            enum class Cipher
            {
                TQ,
                BF,
                RC5
            };

        public:
            virtual ~ICipher() = default;

        public:
            Cipher get_cipher() const { return _cipher; }

        public:
            virtual void encrypt(uint8_t* pt, size_t len) = 0;
            virtual void decrypt(uint8_t* ct, size_t len) = 0;

        protected:
            Cipher _cipher;
        };
    }
}
