#pragma once
#include <cstdint>

namespace Eternal
{
    namespace Encryption
    {
        class TqCipher
        {

		public:
			static const size_t IV_SIZE = 512;

		public:
			TqCipher();
			~TqCipher() = default;;

		public:		
			void generate_iv();
			void encrypt(uint8_t* data, size_t len) ;
			void decrypt(uint8_t* data, size_t len) ;

		public:
			uint32_t get_key1() const { return _key1; }
			uint32_t get_key2() const { return _key2; }

			void set_key1(uint32_t key1) { _key1 = key1; }
			void set_key2(uint32_t key2) { _key2 = key2; }

		private:
			uint32_t _key1;
			uint32_t _key2;

		private:
			uint16_t _en_counter;	
			uint16_t _de_counter;	
			uint8_t _iv[IV_SIZE];	
		};

    }
}


