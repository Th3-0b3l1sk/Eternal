#pragma once
#include <cstdint>
#include <filesystem>

namespace Eternal
{
    namespace Util
    {
        class BinaryRW
        {

            static void _deleter(uint8_t* ptr);

        public:
            typedef std::unique_ptr<uint8_t, decltype(&BinaryRW::_deleter)> unique_deleter;

        public:
            enum class Dir
            {
                Forward,
                Backward
            };
        public:
            BinaryRW(uint8_t* ptr)
                : _ptr{ ptr }, _read{ 0 }, _write{ 0 }, _is_owner{false},
                _owner_ptr{ nullptr, BinaryRW::_deleter }
            {

            }
            BinaryRW(std::filesystem::path file_path);

            ~BinaryRW() {
                _ptr = nullptr;
            }

        public:
            template <typename T>
            T read()
            {
                T value = *(T*)_ptr;
                _ptr += sizeof(T);
                _read += sizeof(T);
                return value;
            }

            template<typename T>
            void write(T value)
            {
                *(T*)_ptr = value;
                _ptr += sizeof(T);
                _write += sizeof(T);
            }

            template<typename T>
            T* cast()
            {
                return (T*)_ptr;
            }

        public:
            void seek_r(size_t offset, BinaryRW::Dir dir = Dir::Forward) { _seek_read(offset, dir); }
            void seek_w(size_t offset, BinaryRW::Dir dir = Dir::Forward) { _seek_write(offset, dir); }

        public:
            unique_deleter&& let_ptr();

        private:
            void _seek_read(size_t offset, BinaryRW::Dir dir)
            {
                switch (dir)
                {
                case Dir::Forward:
                    _read += offset;
                    _ptr += offset;
                    break;
                case Dir::Backward:
                    if (offset > _read)
                        offset = _read;
                    _read -= offset;
                    _ptr -= offset;
                    break;
                }
            }

            void _seek_write(size_t offset, BinaryRW::Dir dir)
            {
                switch (dir)
                {
                case Dir::Forward:
                    _write += offset;
                    _ptr += offset;
                    break;
                case Dir::Backward:
                    if (offset > _write)
                        offset = _write;
                    _write -= offset;
                    _ptr -= offset;
                    break;
                }
            }

        private:
            uint8_t* _ptr;
            size_t _read;
            size_t _write;
            bool   _is_owner;
            unique_deleter _owner_ptr;
        };
    }
}
