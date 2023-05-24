#pragma once
namespace Eternal
{
    namespace Util
    {
        class Packer
        {
        public:
            static int pack(char* src, char* dst, int src_size, int dst_capacity);
            static int unpack(char* src, char* dst, int compressed_size, int dst_capacity);
            static int get_packed_size(size_t unpacked_size);
        };
    }
}


