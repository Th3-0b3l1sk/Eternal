#include "./Network/Encryption/TqCipher.h"
#include <string>

namespace Eternal
{
    namespace Encryption
    {
        TqCipher::TqCipher()
            :_en_counter(0), _de_counter(0),
            _key1(UINT32_C(0x9dfa0f13)), _key2(UINT32_C(0x62a4796d))
        {
            _cipher = Cipher::TQ;
            memset(_iv, 0, TqCipher::IV_SIZE);
        }

        // 004bbbf8 [0x9dfa0f13] [0x62a4796d]
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
        /* Note:
        *   The method in the client that handles the encryption of the the packets using this cipher
        * seems to be the same method that's also used in the decryption. The order of the operations
        * is something like this:
        *   packet[i] ^= Y;
        *   rotl8(packet[i]);
        *   packet[i] ^= 0xAB;
        * Which can be decrypted successfuly using the below TqCipher::decrypt method. However, when 
        * attempting to encrypt using the below TqCipher::encrypt, the game client fails to successfuly
        * decrypt the packet since the order of operations is different. 
        * In short, the function directly below this comment is the identical function used in the client
        * for the encryption, however, it won't be used in the server for the previously mentioned reason.
        * The server function WILL BREAK the TqCipher EncryptDecrypt test.
        */
        /*void TqCipher::encrypt(std::uint8_t* _pBuf, size_t _len)
        {
            uint8_t a = 0;
            for (size_t i = 0; i < _len; ++i)
            {
                _pBuf[i] ^= _iv[_en_counter++];
                _pBuf[i] ^= _iv[a + 0x100];
                _pBuf[i] = (_pBuf[i] << 0x4) + (_pBuf[i] >> 0x4);
                _pBuf[i] ^= UINT8_C(0xAB);
                
                if (_en_counter > 0xff) {
                    _en_counter = 0;
                    a++;
                } if (a > 0xff) {
                    a = 0;
                    
                }
            }
        }*/

        void TqCipher::encrypt(std::uint8_t* _pBuf, size_t _len)
        {
            uint8_t a = 0;
            for (size_t i = 0; i < _len; ++i)
            {   
                _pBuf[i] ^= UINT8_C(0xAB);
                _pBuf[i] = (_pBuf[i] << 0x4) + (_pBuf[i] >> 0x4);
                _pBuf[i] ^= _iv[_en_counter++];
                _pBuf[i] ^= _iv[a + 0x100];
                
                if (_en_counter > 0xff) {
                    _en_counter = 0;
                    a++;
                } if (a > 0xff) {
                    a = 0;
                }
            }
        }

        void TqCipher::decrypt(uint8_t* _pBuf, size_t _len)
        {
            uint8_t b = 0;
            for (size_t i = 0; i < _len; ++i)
            {
                _pBuf[i] ^= UINT8_C(0xAB);
                _pBuf[i] = (_pBuf[i] << 0x4) + (_pBuf[i] >> 0x4);    // the right shift operation is AFFECTED by the sign of its operand (char/uint8_t) 
                                                                     // wasted  ~4 hours on this little bug :))
                _pBuf[i] ^= _iv[_de_counter++];
                _pBuf[i] ^= _iv[b + 0x100];

                if (_de_counter > 0xff) {
                    _de_counter = 0;
                    b++;
                    if (b > 0xff)
                         b = 0;
                } 
            }
        }
    }
}
