#include "Rc5.h"

//TODO: can this implementation be prettier?

// The implementation is hacked together from the wikipedia Rc5 entry https://en.wikipedia.org/wiki/RC5 
// and by reversing the client.
namespace Eternal
{
    namespace Encryption
    {
        Rc5::Rc5()
            :   _p(UINT32_C(0xb7e15163)), 
                _q(UINT32_C(0x9e3779b9)),
            _magics{ UINT32_C(0xE8FEDC3C), UINT32_C(0x7ED654C4),UINT32_C(0x1AF8A616),UINT32_C(0xBE38D0E8) }
        {
            _key = std::make_unique<uint32_t[]>(UINT8_C(KEY_SIZE / 4));
            init_key_expansion();
        }

        // ghidra -> 004e1937
        void Rc5::init_key_expansion()
        {
            for (int i{ 0 }; i < _magics.size(); ++i)
                _key[i] = _magics[i];

            _key[4] = _p;
            auto current = &_key[4];
            for (int i{ 1 }; i < UINT8_C(26); ++i) 
                current[i] = current[i-1] + _q;
            
            uint32_t k = 0;
            uint32_t a = 0;
            uint32_t b = 0;
            uint32_t i = 0;
            uint32_t j = 0;
            uint32_t t = 2 * (ROUNDS + 1);
            uint32_t count = UINT8_C(3 * t);
            do {
                b = _key[i + 4] = rotl_32(a + b + _key[i + 4], 3);
                i = (i + 1) % t;
                a = _key[k] = rotl_32(a + b + _key[k], a + b);                
                j = k + 1;
                k = j % 4;
                --count;
            } while (count != 0);

        }
        
        // ghidra -> 004e1a60
        uint32_t Rc5::rotl_32(uint32_t word, uint8_t n)
        {
            return word << n | word >> (UINT32_C(32) - n);
        }

        uint32_t Rc5::rotr_32(uint32_t word, uint8_t n)
        {
            return word >> n | word << (UINT32_C(32) - n);
        }

        // ghidra -> 004e19d8
        void Rc5::encrypt(uint8_t* data, uint32_t len)
        {
            uint32_t* pt = (uint32_t*)data;
            uint32_t A = 0;
            uint32_t B = 0;
            uint32_t* sub_key = &_key[4];
            
            for (uint32_t qwords = len / 8; qwords > 0; --qwords)
            {
                A = pt[0] + sub_key[0];
                B = pt[1] + sub_key[1];
                for (uint32_t i{ 1 }; i <= ROUNDS; ++i) {
                    A = rotl_32((A ^ B), B) + sub_key[2 * i];
                    B = rotl_32((B ^ A), A) + sub_key[2 * i + 1];
                }

                pt[0] = A;
                pt[1] = B;
                pt += 2;
            }
        }

        // wikipedia :D
        void Rc5::decrypt(uint8_t* data, uint32_t len)
        {
            uint32_t* ct = (uint32_t*)data;
            uint32_t A = 0;
            uint32_t B = 0;
            uint32_t* sub_key = &_key[4];

            for (uint32_t qwords = len / 8; qwords > 0; --qwords)
            {
                A = ct[0];
                B = ct[1];
                for (uint32_t i{ ROUNDS }; i > 0; --i) {
                    B = rotr_32((B - sub_key[2 * i + 1]), A) ^ A;
                    A = rotr_32((A - sub_key[2 * i]), B) ^ B;    
                }

                ct[0] = A - sub_key[0];
                ct[1] = B - sub_key[1];
                ct += 2;
            }
        }
    }
}


