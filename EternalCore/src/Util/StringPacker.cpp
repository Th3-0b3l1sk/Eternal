#include "Util\StringPacker.h"

StringPacker::StringPacker(uint8_t* buffer)
    :
    _buffer(buffer)
{
}

void StringPacker::AddString(std::string str)
{
    uint8_t num_of_strings = *_buffer;
    uint8_t* tmp = _buffer + 1;
    for (uint8_t i{}; i < num_of_strings; ++i)
    {
        tmp += *tmp;    // string length
        tmp += 1;       // for the actual string length byte and the null terminator
    }

    *tmp = str.length();
    tmp++;
    memcpy((char*)tmp, str.c_str(), str.length());
    tmp += str.length();
    *_buffer += 1;
}
