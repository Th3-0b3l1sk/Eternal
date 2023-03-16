#include "NetMsg.h"
#include <sstream>
#include <iomanip>
#include <assert.h>

namespace Eternal
{
    namespace Msg
    {
        NetMsg::NetMsg(uint8_t* buffer, size_t size)
            : _buffer(buffer), _size(size)
        {}

        std::string NetMsg::stringfy()
        {
            assert(_buffer != nullptr, "Attemptin to stringfy a non-existent packet!");
            // format 
            // [type][len]
            // hex dump .... ascii dump 16 per line 
            std::ostringstream buf;
            std::ostringstream ascii;

            buf << '[' << msg_type_to_string(((Header*)_buffer)->type) << "][" << _size << "]\n";
            for (size_t i{}; i < _size; i++) {
                if (i != 0 && i % 15 == 0) {
                    buf << "    " << ascii.str() << '\n';
                    ascii.str("");
                }

                buf << std::setw(2) << std::setfill('0') << std::hex << ((int)_buffer[i] & 0xff )<< ' ';
                if (std::isprint(_buffer[i])) 
                    ascii << _buffer[i];
                else 
                    ascii << ".";

            }

            // for residue bytes < 0x16 limit
            if (auto len = ascii.str().size(); len != 0) {
                size_t len_spaces = 45 - len * 3;   // hacky but it works! 14 * 3(XY ) + 1 
                buf << std::string(len_spaces, ' ') << "    " << ascii.str() << '\n';
            }

            return buf.str();

        }

        void NetMsg::process()
        {

        }

    }
}

