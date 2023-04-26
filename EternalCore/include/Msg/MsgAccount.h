#pragma once
#include "NetMsg.h"

namespace Eternal
{
    namespace Msg 
    {
        class MsgAccount : public NetMsg
        {
        private:
#pragma pack(push , 1)
            struct Info
            {
                Msg::Header header;
                char account_name[16];
                char account_pass[16];
                char server_name[16];
            };

#pragma pack(pop)
        public:
            MsgAccount(std::shared_ptr<uint8_t[]>&& data, size_t len);
            char* get_password() const { return _info->account_pass; }
            uint32_t get_password_len() const { return 0x10; }

        public:
            virtual void process(Server& server, uint32_t con_id) override;

        private:
            Info* _info;
        };
    }

}