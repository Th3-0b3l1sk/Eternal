#pragma once
#include <string>
#include <memory>

namespace Eternal
{
    class Server;
    namespace Msg
    {
        enum class MsgType: uint16_t
        {
            MSG_ACCOUNT = 1051,
            MSG_CONNECT_EX = 1055,
            MSG_CONNECT = 1052,
            MSG_TALK = 1004,

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
            virtual ~NetMsg() {};
            NetMsg(std::shared_ptr<uint8_t[]>&& buffer, size_t size);
            NetMsg(size_t len);

        public:
            std::string stringfy();
            std::shared_ptr<uint8_t[]> get_data() const { return _buffer; }
            std::size_t get_size() const { return _size; }
        public:
            virtual void process(Server& server);
            static std::shared_ptr<NetMsg> create(std::shared_ptr<uint8_t[]>&& data, size_t len);

        private:
            std::string msg_type_to_string(MsgType type) {
                switch (type)
                {
                case MsgType::MSG_ACCOUNT:
                    return "MSG_ACCOUNT";
                case MsgType::MSG_CONNECT_EX:
                    return "MSG_CONNECT_EX";
                case MsgType::MSG_CONNECT:
                    return "MSG_CONNECT";
                case MsgType::MSG_TALK:
                    return "MSG_TALK";
                default:
                    return "UNKNOWN_TYPE";
                }
            }
            
        protected:
            size_t _size;
            std::shared_ptr<uint8_t[]> _buffer;
        };
    }
}

