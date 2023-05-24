#include "Util/Packer.h"
#include "lz4/lz4.h"

namespace Eternal
{
    namespace Util
    {
        int Packer::pack(char* src, char* dst, int src_size, int dst_capacity)
        {
            return LZ4_compress_default(src, dst, src_size, dst_capacity);
        }

        int Packer::unpack(char* src, char* dst, int compressed_size, int dst_capacity)
        {
            return LZ4_decompress_safe(src, dst, compressed_size, dst_capacity);
        }

        int Packer::get_packed_size(size_t unpacked_size)
        {
            return  LZ4_compressBound(unpacked_size);
        }
    }
}
