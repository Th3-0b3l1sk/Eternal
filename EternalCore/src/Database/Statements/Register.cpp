#include "Database/Statements/Register.h"

namespace Eternal
{
    namespace Database
    {
        Register::Register()
            : IStatement(Eternal::Database::StatementID::REGISTER)
        {
            _info = std::make_unique<uint8_t[]>(sizeof(Info));
        }

        Register::Register(std::string_view name, std::string_view password, std::string_view ip, uint32_t creation_time)
            : Register()
        {
            // the user of this statement enforces the rules on name and password
            auto info = (Info*)_info.get();
            info->creation_time = creation_time;
            memcpy_s(info->name, MAX_ACCOUNT_LEN, name.data(), name.length());
            memcpy_s(info->password, MAX_PASSWORD_LEN, password.data(), password.length());
            memcpy_s(info->last_ip, MAX_IP_LEN, ip.data(), ip.length());
        }

        SQLRETURN Register::bind()
        {
            auto info = (Info*)_info.get();

            // bind username
            TRYODBC(_hStatement, SQL_HANDLE_STMT,
                SQLBindParameter(_hStatement, PARAM_ONE, SQL_PARAM_INPUT, C_STR, SQL_VARCHAR, NULL, NULL, 
                    (SQLPOINTER)info->name, strnlen_s((char*)info->name, MAX_ACCOUNT_LEN), NULL));
            // bind password
            TRYODBC(_hStatement, SQL_HANDLE_STMT,
                SQLBindParameter(_hStatement, PARAM_TWO, SQL_PARAM_INPUT, C_STR, SQL_VARCHAR, NULL, NULL, 
                    (SQLPOINTER)info->password, strnlen_s((char*)info->password, MAX_PASSWORD_LEN), NULL));
            // bind  last ip
            TRYODBC(_hStatement, SQL_HANDLE_STMT,
                SQLBindParameter(_hStatement, PARAM_THREE, SQL_PARAM_INPUT, C_STR, SQL_VARCHAR, NULL, NULL,
                    (SQLPOINTER)info->last_ip, strnlen_s((char*)info->last_ip, MAX_IP_LEN), NULL));
            // bind creation time
            TRYODBC(_hStatement, SQL_HANDLE_STMT,
                SQLBindParameter(_hStatement, PARAM_FOUR, SQL_PARAM_INPUT, C_QWORD, SQL_BIGINT, NULL, NULL, 
                    (SQLPOINTER)&info->creation_time, NULL, NULL));

            return SQL_SUCCESS;

        bailout:
            return SQL_ERROR;
        }

        std::vector<std::unique_ptr<uint8_t[]>> Register::fetch()
        {
            // TODO: replace the return with std::optional
            return std::vector<std::unique_ptr<uint8_t[]>>();
        }
    }
}

