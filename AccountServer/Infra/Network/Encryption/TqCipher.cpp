#include "TqCipher.h"
#include <string>

namespace Eternal
{
    namespace Encryption
    {
        TqCipher::TqCipher()
            :_en_counter(0), _de_counter(0),
            _key1(UINT32_C(0x9dfa0f13)), _key2(UINT32_C(0x62a4796d))
        {
            memset(_iv, 0, TqCipher::IV_SIZE);
        }

        // 004bbbf8 [0x9dfa0f13] []
        void TqCipher::generate_iv()
        {
            uint8_t* a = (uint8_t*)&_key1;
            uint8_t* b = (uint8_t*)&_key2;

            uint8_t* block1 = _iv;
            uint8_t* block2 = block1 + (TqCipher::IV_SIZE / 2);

            for (size_t i = 0, len = (TqCipher::IV_SIZE / 2); i < len; ++i)
            {
                block1[i] = a[3];
                block2[i] = b[3];
                a[3] = (uint8_t)((a[1] + (uint8_t)(a[3] * a[2])) * a[3] + a[0]);
                b[3] = (uint8_t)((b[1] + (uint8_t)(b[3] * b[2])) * b[3] + b[0]);
            }
        }

        //004BBC59 encrypt
        void TqCipher::encrypt(std::uint8_t* _pBuf, size_t _len)
        {
            for (size_t i{}; i < _len; i++)
            {
                _pBuf[i] ^= _iv[_en_counter++];
                _pBuf[i] ^= _iv[_de_counter + 0x100];
                _pBuf[i] = (_pBuf[i] << 0x4) + (_pBuf[i] >> 0x4);
                _pBuf[i] ^= 0xab;

                if (_en_counter > 0xff) {
                    _en_counter = 0;
                    _de_counter++;
                    if (_de_counter > 0xff)
                        _de_counter = 0;
                }

            }

        }

        void TqCipher::decrypt(uint8_t* _pBuf, size_t _len)
        {
            uint8_t* key1 = _iv;
            uint8_t* key2 = key1 + (TqCipher::IV_SIZE / 2);

            for (size_t i = 0; i < _len; ++i)
            {
                _pBuf[i] ^= UINT8_C(0xAB);
                _pBuf[i] = (_pBuf[i] << 0x4) + (_pBuf[i] >> 0x4);    // the right shift operation is AFFECTED by the sign of its operand (char/uint8_t) 
                // wasted  ~4 hours on this little bug :))
                _pBuf[i] ^= _iv[_de_counter++];
                _pBuf[i] ^= _iv[_en_counter + 0x100];

                if (_de_counter > 0xff) {
                    _de_counter = 0;
                    _en_counter++;
                    if (_en_counter > 0xff)
                        _en_counter = 0;
                }

            }

        }

    }
}
