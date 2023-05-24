#include "pch.h"
#include "CppUnitTest.h"
#include "Util/StringPacker.h"
#include "Util/LineReader.h"
#include "Util/IniFile.h"
#include "Util/Packer.h"
#include "Network/Encryption/TqCipher.h"
#include "GameServer/Encryption/DiffieHellman.h"
#include "GameServer/Encryption/Blowfish.h"
#include "Map/MapData.h"
#include "Map/MapManager.h"
#include "Map/Grid.h"

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

	TEST_CLASS(LineReader)
	{
	public:
		TEST_METHOD(OpeningFiles)
		{
			// non-existent file
			Assert::ExpectException<std::exception>([]() {
				auto reader = Eternal::Util::LineReader("../EternalTest/_non_existent_test_file.txt");
				reader.get_lines();
				});

			auto reader = Eternal::Util::LineReader("../EternalTest/compliant_format.txt");
		}
		TEST_METHOD(CompliantFile)
		{
			auto reader = Eternal::Util::LineReader("../EternalTest/compliant_format.txt");
			auto data = reader.get_lines();
			auto test_data = std::vector<std::pair<std::string, std::string>>{ { "key1", "value1" }, { "key2 ",  " value2" }, { "key3", "" } };

			Assert::AreEqual(data.size(), test_data.size());
			int i{};
			for (const auto& pair : data) {
				Assert::AreEqual(pair.first, test_data[i].first);
				Assert::AreEqual(pair.second, test_data[i].second);
				i++;
			}
		}
		TEST_METHOD(NonCompliantFile)
		{
			auto reader = Eternal::Util::LineReader("../EternalTest/non_compliant_format.txt");
			Assert::ExpectException<std::exception>([&]() {reader.get_lines(); });
		}
	};

	TEST_CLASS(IniFile)
	{
	public: 
		TEST_METHOD(IniFile_Ctor)
		{
			Assert::ExpectException<std::filesystem::filesystem_error>([]() {
				Eternal::Util::IniFile ini_file("../EternalTest/non_existent_test_ini.ini");
				});

			Assert::ExpectException<std::logic_error>([]() {
				Eternal::Util::IniFile ini_file("../EternalTest/invalid_ini.ini");
				});

			auto ini_file = Eternal::Util::IniFile("../EternalTest/test_ini.ini");
			auto port = ini_file.get<int>("SERVER", "port");
			Assert::AreEqual(port, 55099);
			auto ip = ini_file.get("SERVER", "ip");
			Assert::AreEqual(ip, std::string("192.168.1.1"));
			
			Assert::ExpectException<std::exception>([&ini_file]() {
				ini_file.get("NON_EXISTENT_KEY", "VALYE");
				});

			auto pass = ini_file.get("SETTINGS", "pass");
			Assert::AreEqual(pass, std::string("N0t~a~r3al~p@ss~00"));
			auto root_dir = ini_file.get<double>("GLOBAL", "ROOT_DIR");
			Assert::AreEqual(root_dir, 1.1234);
		}
	};

	TEST_CLASS(Packer)
	{
		TEST_METHOD(pack_unpack)
		{
			// lol it required more bytes to pack the string than
			// the actual bytes used by the actual string
			char data[] = "testing the lz4 packing packer hmmp || ?!";
			const size_t data_len = strlen(data) + 1;
			using Eternal::Util::Packer;

			auto packed_size = Packer::get_packed_size(data_len);

			auto packed_data = (char*)malloc(packed_size);
			auto result  = Packer::pack(data, packed_data, data_len, packed_size);
			Assert::AreNotEqual(result, 0);

			if (result > 0)
				packed_data = (char*)realloc(packed_data, result);

			auto unpacked_data = new char[data_len];
			auto uresult = Packer::unpack(packed_data, unpacked_data, result, (int)data_len);
			for (auto i{ 0 }; i < data_len; i++)
				Assert::AreEqual(data[i], unpacked_data[i]);
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

namespace Map
{
	TEST_CLASS(MapData)
	{
	public:
		TEST_METHOD(LoadFromDmap)
		{
			// todo: fix file to a real dmap
			auto data = Eternal::Map::MapData(1010);
			data.load_data(R"(C:\Dev\Eternal\Depends\street.DMap)");
		}

		TEST_METHOD(pack_unpack)
		{
			auto data = Eternal::Map::MapData(1010);
			data.load_data(R"(C:\Dev\Eternal\Depends\street.DMap)");
			auto bsize = data._get_grid_size();

			data.pack();
			data.unpack();
			auto pp = data._get_raw_grid();

			auto size1 = data._get_grid_size();

			Assert::AreEqual(bsize, size1);

			auto unpacked = Eternal::Map::MapData(1010);
			unpacked.load_data(R"(C:\Dev\Eternal\Depends\street.DMap)");
			auto up = unpacked._get_raw_grid();
			auto size2 = unpacked._get_grid_size();

			Assert::AreEqual(size1, size2);
			for (auto i{ 0 }; i < size1; i++)
				Assert::AreEqual(up[i], pp[i]);
		}
	};

	TEST_CLASS(MapManager)
	{
	public:
		TEST_METHOD(load_maps)
		{
			Eternal::Map::MapManager manager;
			manager.load_maps(R"(C:\Dev\Eternal\EternalTest\GameMap.ini)");
		}
	};

	TEST_CLASS(Grid)
	{
	public:
		TEST_METHOD(set_cell)
		{
			/*
			* * SET THE FIRST ROW TO ALL 1s
			*/

			const uint32_t width  = 10;
			const uint32_t height = 10;

			Eternal::Map::Grid grid(width, height);
			
			Eternal::Map::Cell cell;
			cell.accessible = cell.elevation = cell.surface = 1;

			for (int w = 0; w < width; w++)
				grid.set_cell(cell, 0, w);

			for (int w = 0; w < width; w++) {
				auto cell = grid.get_cell(0, w);
				Assert::AreEqual(cell->accessible, (uint16_t)1);
				Assert::AreEqual(cell->surface,    (uint16_t)1);
				Assert::AreEqual(cell->elevation,  (int16_t)1);
			}
		}

		TEST_METHOD(get_cell)
		{
			/*
			* * SET THE CELL IN THE x ROW, y COL TO ALL 1s
			*/

			const uint32_t width = 10;
			const uint32_t height = 10;

			const uint32_t x = 3;	// 4th row
			const uint32_t y = 5;	// 5th col

			Eternal::Map::Cell c_grid[height][width];
			Eternal::Map::Grid grid(width, height, (uint8_t*)c_grid);
			
			Eternal::Map::Cell cell;
			cell.accessible = cell.elevation = cell.surface = 1;
			
			c_grid[x][y] = cell;

			auto g_cell = grid.get_cell(x, y);

			Assert::AreEqual(g_cell->accessible, (uint16_t)1);
			Assert::AreEqual(g_cell->surface,    (uint16_t)1);
			Assert::AreEqual(g_cell->elevation,  (int16_t)1);
		}
	};
}
