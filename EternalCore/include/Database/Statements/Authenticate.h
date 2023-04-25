#pragma once
#include "./IStatement.h"

namespace Eternal
{
    namespace Database
    {
        class Authenticate :
            public IStatement
        {
        private:
#pragma pack(push, 1)
            struct Info
            {
                Info()
                    : id{ 0 }, name{ "" }, password{ "" }, type{ 0 },
                    creation_time{ 0 }, online_status{ 0 }, last_ip{ "" }
                {
                    ZeroMemory(name, MAX_ACCOUNT_LEN);
                    ZeroMemory(password, MAX_PASSWORD_LEN);
                    ZeroMemory(last_ip, MAX_IP_LEN);
                }

                int32_t  id;
                uint8_t  name[MAX_ACCOUNT_LEN];
                uint8_t  password[MAX_PASSWORD_LEN];
                uint8_t  type;
                uint32_t creation_time;     // why int?
                uint8_t  online_status;
                uint8_t  last_ip[MAX_IP_LEN];
            };
#pragma pack(pop)
        public:
            Authenticate();
            virtual ~Authenticate() = default;
            virtual SQLRETURN  bind() override;
            virtual std::vector<std::unique_ptr<uint8_t[]>> fetch() override;
        public:
            // TODO: is this a bad design?
            void set_name(std::string_view name) {
                auto ptr = (Info*)_info.get();
                memcpy_s(ptr->name, MAX_ACCOUNT_LEN, name.data(), name.size());
            }

            std::unique_ptr<Info>&& get_info() {
                auto ptr = (Info*)_info.get();
                _info.reset(nullptr);
                auto ret = std::make_unique<Info>();
                ret.reset(ptr);
                return std::move(ret);
            }

        private:
            const char* get_name() const {
                auto ptr = (Info*)_info.get();
                return (const char*)ptr->name;
            }

        private:

        };

    }
}
