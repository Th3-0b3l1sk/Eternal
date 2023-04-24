#define _CRT_SECURE_NO_WARNINGS
#include "Database\Database.h"
#include <string>
#include <iostream>

/*******************************************/
/* Macro to call ODBC functions and        */
/* report an error on failure.             */
/* Takes handle, handle type, and stmt     */
/*******************************************/
#define TRYODBC(handle, handle_type, fun)                           \
    {                                                               \
        RETCODE rc = fun;                                           \
        if(rc != SQL_SUCCESS) {                                     \
            auto error_msg = get_error(handle, handle_type, rc);    \
            std::cout << "[database_error] " << error_msg << '\n';  \
        }                                                           \
        if(rc == SQL_ERROR) {                                       \
            std::cout << "Error in " # fun << '\n'; goto bailout; } \
    }                                                               \
        

#define DO_IF(cond, what, ...) if(cond) what(__VA_ARGS__);

namespace Eternal
{
    Database::Database()
        : _hEnv(SQL_NULL_HENV), _hCon(SQL_NULL_HDBC)
    {
        TRYODBC(_hEnv, SQL_HANDLE_ENV, 
            SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &_hEnv));
        TRYODBC(_hEnv, SQL_HANDLE_ENV,
            SQLSetEnvAttr(_hEnv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0));
        TRYODBC(_hCon, SQL_HANDLE_DBC,
            SQLAllocHandle(SQL_HANDLE_DBC, _hEnv, &_hCon));

        return;

    // TRYODBC goes to bailout on failure
    bailout:
        DO_IF(_hEnv != SQL_NULL_HANDLE, SQLFreeHandle, SQL_HANDLE_ENV, _hEnv);
        DO_IF(_hCon != SQL_NULL_HANDLE, SQLFreeHandle, SQL_HANDLE_DBC, _hCon);

        throw std::exception{ "panic" };
    }

    Database::Database(std::string_view dsn_name, std::string_view username, std::string_view password)
        : Database()
    {
        if (_hEnv == SQL_NULL_HANDLE || _hCon == SQL_NULL_HANDLE)
            return;

        TRYODBC(_hCon, SQL_HANDLE_DBC,
            SQLConnectA(_hCon,
                (SQLCHAR*)dsn_name.data(),  dsn_name.size(),
                (SQLCHAR*)username.data(),  username.size(),
                (SQLCHAR*)password.data(),  password.size()));

        std::cout << "[successfully connected to the database!]\n";
        return;

    bailout:
        DO_IF(_hCon != SQL_NULL_HANDLE, SQLFreeHandle, SQL_HANDLE_DBC, _hCon);
        throw std::exception{ "panic" };
    }

    /* Yanked straight out from: https://learn.microsoft.com/en-us/sql/connect/odbc/cpp-code-example-app-connect-access-sql-db?view=sql-server-ver16 */
    std::string Database::get_error(SQLHANDLE hHandle, SQLSMALLINT hType, RETCODE return_code)
    {
        SQLSMALLINT iRec = 0;
        SQLINTEGER  iError;
        std::string error_message;
        std::string error_state;
        std::string error;

        error_message.resize(1000);
        error_state.resize(SQL_SQLSTATE_SIZE + 1);
        error.resize(error_message.capacity() + error_state.capacity());
        auto szMessage  = &error_message[0];
        auto szState    = &error_state[0];
        auto e          = &error[0];

        if (return_code == SQL_INVALID_HANDLE)
            return { "Invalid Handle!" };

        while (SQLGetDiagRec(hType,
            hHandle,
            ++iRec,
            (SQLCHAR*)szState,
            &iError,
            (SQLCHAR*)szMessage,
            error_message.size(),
            (SQLSMALLINT*)NULL) == SQL_SUCCESS)
        {
            // Hide data truncated..
            if (strncmp(szState, "01004", 5))
            {
                sprintf(e, "[%5.5s] %s (%d)\n", szState, szMessage, iError);
                return std::move(error);
            }
        }
    }
}
