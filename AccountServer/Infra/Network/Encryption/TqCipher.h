#pragma once
#include <cstdint>

namespace Eternal
{
    namespace Encryption
    {
        class TqCipher
        {

		public:
			/* the initialization vector size in bytes. Used as the primary key. */
			static const size_t IV_SIZE = 512;

		public:
			TqCipher();
			~TqCipher() = default;;

		public:
			/**
			 * @brief generate the initalization vector based on the values of P & G
			 * which are 32-bit each.
			 * @param _P the P of the cipher.
			 * @param _G the G of the cipher.
			*/
			void generate_iv(int32_t _P, int32_t _G);

			void encrypt(std::uint8_t* _pBuf, size_t _len) ;

			void decrypt(uint8_t* _pBuf, size_t _len) ;

		public:
			// virtual ICipher::Algorithm GetAlgorithm() const override { return TQ_CIPHER; }

		private:
			uint16_t _en_counter;	// internal encryption counter.
			uint16_t _de_counter;	// internal decryption counter.

			uint8_t _iv[IV_SIZE];	// the initialization vector.

		};

    }
}


