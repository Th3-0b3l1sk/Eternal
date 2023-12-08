#include "Database/Statements/GetAccountInfo.h"

namespace Eternal
{
    namespace Database
    {
        GetAccountInfo::GetAccountInfo(SQLHANDLE hCon)
            : _type{ 0 }, _is_online{ false }, _id{ 0 }
        {
            SQLAllocHandle(SQL_HANDLE_STMT, hCon, &_hStmt);

            _name.resize(MAX_ACCOUNT_LEN);
            _password.resize(MAX_PASSWORD_LEN);
            _ip.resize(MAX_IP_LEN);
        }


        SQLRETURN  GetAccountInfo::bind()
        {
            // prepare the statement
            // get_account_info(name, id[O], password[O], type[O], last_ip[O], isOnline[O])
            static std::string stmt = "EXEC get_account_info @name=?, @id=?, @password=?, @type=?, @last_ip=?, @isOnline=?";
            TRYODBC(_hStmt, SQL_HANDLE_STMT,
                SQLPrepare(_hStmt, (SQLCHAR*)stmt.c_str(), stmt.size()));

            // bind account
            TRYODBC(_hStmt, SQL_HANDLE_STMT,
                SQLBindParameter(_hStmt, PARAM_ONE, SQL_PARAM_INPUT, C_STR, SQL_VARCHAR, NULL, NULL,
                    (SQLPOINTER)_name.c_str(), _name.size(), NULL));

            // 1- bind id
            TRYODBC(_hStmt, SQL_HANDLE_STMT,
                SQLBindParameter(_hStmt, PARAM_TWO, SQL_PARAM_OUTPUT, C_DWORD, SQL_INTEGER, NULL, NULL,
                    (SQLPOINTER)&_id, sizeof(_id), NULL));
            // 2- bind password
            TRYODBC(_hStmt, SQL_HANDLE_STMT,
                SQLBindParameter(_hStmt, PARAM_THREE, SQL_PARAM_OUTPUT, C_STR, SQL_VARCHAR, NULL, NULL,
                    (SQLPOINTER)_password.c_str(), _password.size(), NULL));
            // 3- bind type
            TRYODBC(_hStmt, SQL_HANDLE_STMT,
                SQLBindParameter(_hStmt, PARAM_FOUR, SQL_PARAM_OUTPUT, C_BYTE, SQL_TINYINT, NULL, NULL,
                    (SQLPOINTER)&_type, sizeof(_type), NULL));
            // 4- bind  last_ip
            TRYODBC(_hStmt, SQL_HANDLE_STMT,
                SQLBindParameter(_hStmt, PARAM_FIVE, SQL_PARAM_OUTPUT, C_STR, SQL_VARCHAR, NULL, NULL,
                    (SQLPOINTER)_ip.c_str(), _ip.size(), NULL));
            // 5- bind  is_online
            TRYODBC(_hStmt, SQL_HANDLE_STMT,
                SQLBindParameter(_hStmt, PARAM_SIX, SQL_PARAM_OUTPUT, C_BYTE, SQL_C_TINYINT, NULL, NULL,
                    (SQLPOINTER)&_is_online, sizeof(_is_online), NULL));

            return SQL_SUCCESS;

        bailout:
            return SQL_ERROR;
        }

        SQLRETURN GetAccountInfo::execute()
        {
            return SQLExecute(_hStmt);
        }
    }
}
