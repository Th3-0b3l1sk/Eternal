#pragma once
#include <Windows.h>
#include <sqlext.h>
#include <sql.h>
#include <string>
#include <iostream> // TODO: proper logging

#pragma warning(disable: 4267)
#pragma warning(disable: 4715)


/*******************************************/
/* Macro to call ODBC functions and        */
/* report an error on failure.             */
/* Takes handle, handle type, and stmt     */
/*******************************************/

 /* Yanked straight out from: https://learn.microsoft.com/en-us/sql/connect/odbc/cpp-code-example-app-connect-access-sql-db?view=sql-server-ver16 */
static std::string get_error(SQLHANDLE hHandle, SQLSMALLINT hType, RETCODE return_code)
{
    SQLSMALLINT iRec = 0;
    SQLINTEGER  iError;
    std::string error_message;
    std::string error_state;
    std::string error;

    error_message.resize(1000);
    error_state.resize(SQL_SQLSTATE_SIZE + 1);
    error.resize(error_message.capacity() + error_state.capacity());
    auto szMessage = &error_message[0];
    auto szState = &error_state[0];
    auto e = &error[0];

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


#define DO_IF(cond, what, ...) if((cond)) what(__VA_ARGS__);


// C-Value Types
#define C_QWORD             SQL_C_UBIGINT       // QWORD
#define C_DWORD             SQL_C_ULONG         // DWORD
#define C_WORD              SQL_C_USHORT        // WORD
#define C_BYTE              SQL_C_UTINYINT      // BYTE
#define C_STR               SQL_C_CHAR          // C_STR

// SQL-Value Types
#define SQL_C               SQL_CHAR            // Character string of fixed string length n.

// Parameter Numbers
#define PARAM_ONE           1
#define PARAM_TWO           2
#define PARAM_THREE         3
#define PARAM_FOUR          4
#define PARAM_FIVE          5
#define PARAM_SIX           6
#define PARAM_SEVEN         7
#define PARAM_EIGHT         8
#define PARAM_NINE          9
#define PARAM_TEN           10

// Column Numbers
#define COL_ONE             1
#define COL_TWO             2
#define COL_THREE           3
#define COL_FOUR            4
#define COL_FIVE            5
#define COL_SIX             6
#define COL_SEVEN           7
#define COL_EIGHT           8
#define COL_NINE            9
#define COL_TEN             10



// Coquer Specific Values
#define MAX_ACCOUNT_LEN     16
#define MAX_PASSWORD_LEN    16
#define MAX_IP_LEN          15