#include "Util/BinaryRW.h"
#include <string>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace Eternal
{
    namespace Util
    {

        void BinaryRW::_deleter(uint8_t* ptr)
        {
            UnmapViewOfFile((LPVOID)ptr);
        }


        BinaryRW::BinaryRW(std::filesystem::path file_path)
            : _ptr{ nullptr }, _read{ 0 }, _write{ 0 }, _is_owner{ true }, 
            _owner_ptr { nullptr, BinaryRW::_deleter }
        {
            if (!std::filesystem::exists(file_path))
                throw std::exception{ "Failed to locate the map file\n" };

            // TODO: should be GENERIC_READWRITE
            HANDLE hFile = CreateFileA(file_path.string().c_str(), GENERIC_READ | GENERIC_WRITE,
                FILE_SHARE_READ, NULL,
                OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,
                NULL);
            if (hFile == INVALID_HANDLE_VALUE) {
                auto msg = "Failed to create a file handle. Error code: " + std::to_string(GetLastError());
                throw std::exception{ msg.c_str() };
            }

            auto hMapping = CreateFileMapping(hFile, NULL,
                PAGE_READWRITE, 0, 0, NULL);
            if (hMapping == INVALID_HANDLE_VALUE || hMapping == NULL) {
                auto msg = "Failed to create a file mapping object. Error code: " + std::to_string(GetLastError());
                throw std::exception{ msg.c_str() };
            }

            auto file = MapViewOfFile(hMapping, FILE_MAP_COPY, 0, 0, 0);
            if (file == NULL) {
                auto msg = "Failed to create a file mapping object. Error code: " + std::to_string(GetLastError());
                throw std::exception{ msg.c_str() };
            }

            CloseHandle(hFile);
            CloseHandle(hMapping);

            _ptr = (uint8_t*)file;
            _owner_ptr.reset((uint8_t*)file);
        }
        
        BinaryRW::unique_deleter&& BinaryRW::let_ptr()
        {
            if (_is_owner) {
                _is_owner = false;
                _ptr = nullptr;
                return std::move(_owner_ptr);
            }
            
            throw std::exception{ "Attempting to move a ptr that's not owned by the reader\n" };
        }
    }
}


