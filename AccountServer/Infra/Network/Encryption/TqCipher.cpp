#include "TqCipher.h"
#include <string>

Eternal::Encryption::TqCipher::TqCipher()
    :_en_counter(0), _de_counter(0)
{
    memset(_iv, 0, IV_SIZE);
}

// ma_maybe_iv_init -> ghidra
void Eternal::Encryption::TqCipher::generate_iv(int32_t aP, int32_t aG)
{
    uint8_t* p = (uint8_t*)&aP;
    uint8_t* g = (uint8_t*)&aG;

    uint8_t* key1 = _iv;
    uint8_t* key2 = key1 + (TqCipher::IV_SIZE / 2);

    for (size_t i = 0, len = (TqCipher::IV_SIZE / 2); i < len; ++i)
    {
        key1[i] = p[0];
        key2[i] = g[0];
        p[0] = (uint8_t)((p[1] + (uint8_t)(p[0] * p[2])) * p[0] + p[3]);
        g[0] = (uint8_t)((g[1] - (uint8_t)(g[0] * g[2])) * g[0] + g[3]);
    }
}

//004BBC59 encrypt
void Eternal::Encryption::TqCipher::encrypt(std::uint8_t* _pBuf, size_t _len)
{
    for (size_t i{}; i < _len; i++) 
    {
        _pBuf[i] ^= _iv[_en_counter++];
        _pBuf[i] ^= _iv[_de_counter + 0x100];
        _pBuf[i]  = (_pBuf[i] << 0x4) + (_pBuf[i] >> 0x4);
        _pBuf[i] ^= 0xab;

        if (_en_counter > 0xff) {
            _en_counter = 0;
            _de_counter++;
            if (_de_counter > 0xff)
                _de_counter = 0;
        }
            
    }

}

void Eternal::Encryption::TqCipher::decrypt(uint8_t* _pBuf, size_t _len)
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
