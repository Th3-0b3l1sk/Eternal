#include <string>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "Util/BinaryRW.h"

namespace Eternal
{
    namespace Util
    {

        void BinaryRW::_deleter(uint8_t* ptr)
        {
            UnmapViewOfFile((LPVOID)ptr);
        }

        BinaryRW::BinaryRW(std::filesystem::path file_path)
            : BinaryRW()
        {
            auto err = _do_open(file_path);
            std::string msg = "";

            switch (err)
            {
            case BRWError::E_FILE_DOESNT_EXIST:
            {
                msg = "BinaryRW::BinaryRW() The file at " + file_path.string() + " doesn't exist.\n";
                break;
            }
            case BRWError::E_FAILED_TO_OPEN_FILE:
            {
                msg = "BinaryRW::BinaryRW() can't open the file at " + file_path.string() + '\n';
                break;
            }
            case BRWError::E_FAILED_TO_MEMMAP:
            {
                msg = "BinaryRW::BinaryRW() failed to create a file mapping for the file " + file_path.string() + '\n';
                break;
            }
            default:
                return;
            }

            throw std::exception{ msg.c_str() };
        }

  

        BinaryRW::BinaryRW()
            : _ptr{ nullptr }, _read{ 0 }, _write{ 0 }, _is_owner{ true },
            _owner_ptr{ nullptr, BinaryRW::_deleter }
        {
        }

        bool BinaryRW::open(std::filesystem::path file_path)
        {
            return BRWError::E_SUCCESS == _do_open(file_path);
        }

        BinaryRW::BRWError BinaryRW::_do_open(std::filesystem::path file_path)
        {
            if (!std::filesystem::exists(file_path)) 
                return BRWError::E_FILE_DOESNT_EXIST;

            // TODO: should be GENERIC_READWRITE
            HANDLE hFile = CreateFileA(file_path.string().c_str(), GENERIC_READ | GENERIC_WRITE,
                FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
                OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,
                NULL);
            if (hFile == INVALID_HANDLE_VALUE)
                return BRWError::E_FAILED_TO_OPEN_FILE;

            auto hMapping = CreateFileMapping(hFile, NULL,
                PAGE_WRITECOPY, 0, 0, NULL);
            if (hMapping == INVALID_HANDLE_VALUE || hMapping == NULL)
                return BRWError::E_FAILED_TO_MEMMAP;

            auto file = MapViewOfFile(hMapping, FILE_MAP_COPY, 0, 0, 0);
            if (file == NULL) 
                return BRWError::E_FAILED_TO_MEMMAP;

            CloseHandle(hFile);
            CloseHandle(hMapping);

            _ptr = (uint8_t*)file;
            _owner_ptr.reset((uint8_t*)file);

            return BRWError::E_SUCCESS;
        }
        
        BinaryRW::unique_deleter&& BinaryRW::let_ptr()
        {
            if (_is_owner) {
                _is_owner = false;
                _ptr = nullptr;
                return std::move(_owner_ptr);
            }
            
            throw std::exception{ "BinaryRW::let_ptr() Attempting to move a ptr that's not owned by the reader\n" };
        }
    }
}


