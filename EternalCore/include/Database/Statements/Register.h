#pragma once
#include "IStatement.h"

namespace Eternal
{
    namespace Database
    {
        class Register :
            public IStatement
        {
        private:
#pragma pack(push, 1)
            struct Info
            {
                Info()
                    :  creation_time{ 0 }
                {
                    ZeroMemory(name, MAX_ACCOUNT_LEN);
                    ZeroMemory(password, MAX_PASSWORD_LEN);
                    ZeroMemory(last_ip, MAX_IP_LEN);
                }


                uint8_t  name[MAX_ACCOUNT_LEN];
                uint8_t  password[MAX_PASSWORD_LEN];
                uint64_t creation_time;             // the local server time(system_clock::now()))
                uint8_t  last_ip[MAX_IP_LEN];
            };
#pragma pack(pop)
        public:
            Register();
            Register(std::string_view name, std::string_view password, std::string_view ip, uint32_t creation_time);
            virtual ~Register() = default;
            virtual SQLRETURN bind() override;
            virtual std::vector<std::unique_ptr<uint8_t[]>> fetch() override;
        };
    }
}




