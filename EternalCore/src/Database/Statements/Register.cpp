#include "Database/Statements/Register.h"

namespace Eternal
{
    namespace Database
    {
        Register::Register(SQLHANDLE hCon)
            : _hStmt {NULL}, _type{ 0 } 
        {
            SQLAllocHandle(SQL_HANDLE_STMT, hCon, &_hStmt);

            _name.resize(MAX_NAME_LEN);
            _password.resize(MAX_PASSWORD_LEN);
            _ip.resize(MAX_IP_LEN);
        }

    
        SQLRETURN Register::bind()
        {
            // prepare the statement
            // register(name, password, type, last_ip)
            static std::string stmt = "EXEC register @name=?, @password=?, @type=?, @last_ip=?";
            TRYODBC(_hStmt, SQL_HANDLE_STMT,
                SQLPrepare(_hStmt, (SQLCHAR*)stmt.c_str(), stmt.size()));
            // bind account
            TRYODBC(_hStmt, SQL_HANDLE_STMT,
                SQLBindParameter(_hStmt, PARAM_ONE, SQL_PARAM_INPUT, C_STR, SQL_VARCHAR, NULL, NULL, 
                    (SQLPOINTER)_name.c_str(), _name.size(), NULL));
            // bind password
            TRYODBC(_hStmt, SQL_HANDLE_STMT,
                SQLBindParameter(_hStmt, PARAM_TWO, SQL_PARAM_INPUT, C_STR, SQL_VARCHAR, NULL, NULL, 
                    (SQLPOINTER)_password.c_str(), _password.size(), NULL));
            // bind type
            TRYODBC(_hStmt, SQL_HANDLE_STMT,
                SQLBindParameter(_hStmt, PARAM_THREE, SQL_PARAM_INPUT, C_BYTE, SQL_TINYINT, NULL, NULL,
                    &_type, sizeof(_type), NULL));
            // bind  last_ip
            TRYODBC(_hStmt, SQL_HANDLE_STMT,
                SQLBindParameter(_hStmt, PARAM_FOUR, SQL_PARAM_INPUT, C_STR, SQL_VARCHAR, NULL, NULL,
                    (SQLPOINTER)_ip.c_str(), _ip.size(), NULL));

            return SQL_SUCCESS;

        bailout:
            return SQL_ERROR;
        }

        SQLRETURN Register::execute()
        {
            return SQLExecute(_hStmt);
        }
    }
}
