#pragma once
#define _CRT_SECURE_NO_WARNINGS 1
#include <cstdint>
#include <string>

class StringPacker
{
public:
    StringPacker(uint8_t* buffer);
public:
    void AddString(std::string str);

private:
    uint8_t* _buffer;   //   thats where the count will be 
};

