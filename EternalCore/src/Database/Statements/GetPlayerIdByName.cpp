#include "Database/Statements/GetPlayerIdByName.h"

namespace Eternal
{
    namespace Database
    {
        GetPlayerIdByName::GetPlayerIdByName(SQLHANDLE hCon)
            : _hStmt(NULL), _player_id(0)
        {
            SQLAllocHandle(SQL_HANDLE_STMT, hCon, &_hStmt);
            _player_name.resize(MAX_NAME_LEN);
        }

        SQLRETURN GetPlayerIdByName::bind()
        {
            // get_player_id_by_name(player_name)
            SQLRETURN ret = SQL_SUCCESS;
            static std::string stmt = "EXEC get_player_id_by_name @player_name=?";
            
            ret = SQLPrepare(_hStmt, (SQLCHAR*)stmt.c_str(), stmt.size());
            if (!SQL_SUCCEEDED(ret)) {
                // TODO: handle error;
                return SQL_ERROR;
            }
            // bind parameters
            TRYODBC(_hStmt, SQL_HANDLE_STMT,
                SQLBindParameter(_hStmt, PARAM_ONE, SQL_PARAM_INPUT, C_STR, SQL_VARCHAR, NULL, NULL,
                    (SQLPOINTER)_player_name.c_str(), _player_name.size(), NULL));

            // bind output
            TRYODBC(_hStmt, SQL_HANDLE_STMT,
                SQLBindCol(_hStmt, 1, C_DWORD, (SQLPOINTER)&_player_id, sizeof(_player_id), NULL);)

            return ret;
        bailout:
            return SQL_ERROR;
        }
        std::uint32_t GetPlayerIdByName::execute()
        {
            if (!SQL_SUCCEEDED(bind()) ){
                return -1;
            }
            
            SQLRETURN ret = SQL_SUCCESS;
            ret = SQLExecute(_hStmt);
            if (!SQL_SUCCEEDED(ret)) {
                // todo: handle error
                return -1;
            }

            ret = SQLFetch(_hStmt);
            if (!SQL_SUCCEEDED(ret)) {
                // the player with that name doesn't exist
                return -1;
            }

            return _player_id;
        }
    }
}