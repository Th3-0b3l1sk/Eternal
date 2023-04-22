#include "pch.h"
#include "CppUnitTest.h"
#include "Util/StringPacker.h"
#include "Network/Encryption/TqCipher.h"
#include "GameServer/Encryption/DiffieHellman.h"
#include "GameServer/Encryption/Blowfish.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Utilities
{
	TEST_CLASS(StringPacker)
	{
	public:
		TEST_METHOD(AddString)
		{
			// the data ptr points to the following structure:
			// TotalNumberOfStrings-LenOfStr0-Str0-LenOfStr1-Str1-.....
			uint8_t* data = new uint8_t[0x200]{};
			Eternal::Util::StringPacker s(data);
			s.AddString("testing_the_packer");
			s.AddString("Using_thorough_tests");
			s.AddString("To_ensure_its_quailty");

			int total_number = *(uint8_t*)data;
			Assert::AreEqual(3, total_number, L"The total number of strings didn't match the expected number");
			delete[] data;
		}
	};

}

namespace Encryption
{
	TEST_CLASS(TqCipher)
	{
	public:
		TEST_METHOD(EncryptDecrypt)
		{
			Eternal::Encryption::TqCipher cipher;
			cipher.generate_iv();

			const size_t len = 0x200;
			uint8_t original[len]{};
			uint8_t data[len]{};
			uint8_t decrypted[len]{};
			
			for (size_t i = 0; i < len; ++i) {
				data[i] = static_cast<uint8_t>(rand() % 256);
				original[i] = data[i];
			}

			cipher.encrypt(data, len);
			cipher.decrypt(data, len);

			for (size_t i = 0; i < len; ++i) {
				Assert::AreEqual(data[i], original[i]);
			}
		}
	};

	TEST_CLASS(Diffie_Hellman)
	{
	public:
		TEST_METHOD(DHExchange)
		{
			// the same set used in the sever
			const std::string P{ "E7A69EBDF105F2A6BBDEAD7E798F76A209AD73FB466431E2E7352ED262F8C558F10BEFEA977DE9E21DCEE9B04D245F300ECCBBA03E72630556D011023F9E857F" };
			const std::string G{ "05" };

			DiffieHellman DH1{ P, G }, DH2{ P, G };
			DH1.generate_key();
			DH2.generate_key();

			auto shared_1 = DH1.derive_shared_secret(DH2.get_public_key());
			auto shared_2 = DH2.derive_shared_secret(DH1.get_public_key());

			Assert::AreEqual(shared_1, shared_2);
		}
	};

	TEST_CLASS(Blow_fish)
	{
	public:
		TEST_METHOD(EncryptDecrypt)
		{
			std::string key = "DR654dt34trg4UI6";
			Blowfish bf(key);
			
			const size_t len = 0x200;
			uint8_t original[len]{};
			uint8_t data[len]{};
			uint8_t decrypted[len]{};

			for (size_t i = 0; i < len; ++i) {
				data[i] = static_cast<uint8_t>(rand() % 256);
				original[i] = data[i];
			}

			bf.encrypt(data, len);
			bf.decrypt(data, len);

			for (size_t i = 0; i < len; ++i) {
				Assert::AreEqual(data[i], original[i]);
			}


		}
	};
}
