#pragma once
#include "Msg/NetMsg.h"
#include "Database/db_helper.h"

namespace Eternal
{
    namespace Msg
    {
        class MsgRegister : public NetMsg
        {
        public:
#pragma pack(push, 1)
            struct Info
            {
                Msg::Header header;
                uint8_t account[MAX_ACCOUNT_LEN];
                uint8_t name[MAX_NAME_LEN];
                uint8_t password[MAX_PASSWORD_LEN];
                uint16_t model;
                uint16_t profession;
                uint32_t identity;
            };
#pragma pack(pop)

        public:
            MsgRegister(std::shared_ptr<uint8_t[]>&& data, size_t len);
            virtual ~MsgRegister() = default;

        public:
            virtual void process(Server& server, uint32_t con_id) override;

        private:
            Info* _info;
        };
    }
}
