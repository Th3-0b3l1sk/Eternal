#pragma once
#include <string>

namespace Eternal
{
    namespace Msg
    {
        enum class MsgType: uint16_t
        {
            MSG_ACCOUNT = 1051,
        };

#pragma pack(push, 1)
        struct Header
        {
            uint16_t length;
            MsgType type;
        };

#pragma pack(pop)
        class NetMsg
        {
        public:
            virtual ~NetMsg() = default;
            NetMsg(uint8_t* buffer, size_t size);
            
        public:
            std::string stringfy();

        public:
            virtual void process();

        private:
            std::string msg_type_to_string(MsgType type) {
                switch (type)
                {
                case MsgType::MSG_ACCOUNT:
                    return "MSG_ACCOUNT";
                default:
                    return "UNKNOWN_TYPE";
                }
            }
            
        protected:
            size_t _size;
            uint8_t* _buffer; // NetMsg doesn't own the buffer
        };
    }
}

