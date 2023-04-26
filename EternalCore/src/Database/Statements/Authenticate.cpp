#include "Database/Statements/Authenticate.h"

namespace Eternal
{
    namespace Database
    {
        Authenticate::Authenticate()
            : IStatement(Eternal::Database::StatementID::AUTHENTICATE)
        {
            _info = std::make_unique<uint8_t[]>(sizeof(Info));
        }

        SQLRETURN  Authenticate::bind()
        {
            return SQLBindParameter(_hStatement,
                PARAM_ONE,
                SQL_PARAM_INPUT, C_STR, SQL_C,
                strnlen_s(get_name(), MAX_ACCOUNT_LEN), NULL,
                (SQLPOINTER)get_name(),
                MAX_ACCOUNT_LEN, NULL);
        }

        // TODO: Use std::optional?
        std::vector<std::unique_ptr<uint8_t[]>> Authenticate::fetch()
        {          
            SQLRETURN rc = SQL_NO_DATA;
            rc = SQLFetch(_hStatement);
            if (rc == SQL_NO_DATA)
                return {};
            
            auto data = std::vector<std::unique_ptr<uint8_t[]>>{};
            Info* ptr = (Info*)new uint8_t[sizeof(Info)]{};

            TRYODBC(_hStatement, SQL_HANDLE_STMT, 
                SQLGetData(_hStatement, COL_ONE,   C_DWORD, &ptr->id,           sizeof(DWORD),    NULL));
            TRYODBC(_hStatement, SQL_HANDLE_STMT,
                SQLGetData(_hStatement, COL_TWO,   C_STR,   ptr->name,          MAX_ACCOUNT_LEN,  NULL));
            TRYODBC(_hStatement, SQL_HANDLE_STMT,
                SQLGetData(_hStatement, COL_THREE, C_STR,   ptr->password,      MAX_PASSWORD_LEN, NULL));
            TRYODBC(_hStatement, SQL_HANDLE_STMT,
                SQLGetData(_hStatement, COL_FOUR,  C_BYTE,  &ptr->type,          0,               NULL));
            TRYODBC(_hStatement, SQL_HANDLE_STMT,
                SQLGetData(_hStatement, COL_FIVE,  C_DWORD, &ptr->creation_time, 0,               NULL));
            TRYODBC(_hStatement, SQL_HANDLE_STMT,
                SQLGetData(_hStatement, COL_SIX,   C_BYTE,  &ptr->online_status, 0,               NULL));
            TRYODBC(_hStatement, SQL_HANDLE_STMT,
                SQLGetData(_hStatement, COL_SEVEN, C_STR,   ptr->last_ip,       MAX_IP_LEN,       NULL));
            
            SQLCloseCursor(_hStatement);
            data.emplace_back((uint8_t*)ptr);
            return std::move(data);

        bailout:
            SQLCloseCursor(_hStatement);
            return {};

        }
    }
}
