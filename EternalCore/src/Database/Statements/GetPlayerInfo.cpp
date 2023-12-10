#include "Database/Statements/GetPlayerInfo.h"

namespace Eternal
{
    namespace Database
    {
        GetPlayerInfo::GetPlayerInfo(SQLHANDLE hCon)
            : _player_info {}, _hStmt{NULL}
        {
            SQLAllocHandle(SQL_HANDLE_STMT, hCon, &_hStmt);
            _player_info.name.resize(MAX_NAME_LEN);
            _player_info.mate.resize(MAX_NAME_LEN);
        }

        SQLRETURN GetPlayerInfo::bind()
        {
            // prepare the statement
            // get_player_info(player_id)
            static std::string stmt = "EXEC get_player_info @player_id=?";
            TRYODBC(_hStmt, SQL_HANDLE_STMT,
                SQLPrepare(_hStmt, (SQLCHAR*)stmt.c_str(), stmt.size()));

            TRYODBC(_hStmt, SQL_HANDLE_STMT,
                SQLBindCol(_hStmt, 2,  SQL_C_LONG,    (SQLPOINTER)&_player_info.identity,       0, nullptr));
            TRYODBC(_hStmt, SQL_HANDLE_STMT,
                SQLBindCol(_hStmt, 3,  SQL_C_CHAR,    (SQLPOINTER)_player_info.name.c_str(), _player_info.name.size(), nullptr));
            TRYODBC(_hStmt, SQL_HANDLE_STMT,
                SQLBindCol(_hStmt, 4,  SQL_C_CHAR,    (SQLPOINTER)_player_info.mate.c_str(), _player_info.mate.size(), nullptr));
            TRYODBC(_hStmt, SQL_HANDLE_STMT,
                SQLBindCol(_hStmt, 5,  SQL_C_LONG,    (SQLPOINTER)&_player_info.lookface,       0, nullptr));
            TRYODBC(_hStmt, SQL_HANDLE_STMT,
                SQLBindCol(_hStmt, 6,  SQL_C_SSHORT,  (SQLPOINTER)&_player_info.hair,           0, nullptr));
            TRYODBC(_hStmt, SQL_HANDLE_STMT,
                SQLBindCol(_hStmt, 7,  SQL_C_LONG,    (SQLPOINTER)&_player_info.money,          0, nullptr));
            TRYODBC(_hStmt, SQL_HANDLE_STMT,
                SQLBindCol(_hStmt, 8,  SQL_C_LONG,    (SQLPOINTER)&_player_info.money_saved,    0, nullptr));
            TRYODBC(_hStmt, SQL_HANDLE_STMT,
                SQLBindCol(_hStmt, 9,  SQL_C_LONG,    (SQLPOINTER)&_player_info.cps,            0, nullptr));
            TRYODBC(_hStmt, SQL_HANDLE_STMT,
                SQLBindCol(_hStmt, 10, SQL_C_TINYINT, (SQLPOINTER)&_player_info.level,          0, nullptr));
            TRYODBC(_hStmt, SQL_HANDLE_STMT,
                SQLBindCol(_hStmt, 11, SQL_C_SBIGINT, (SQLPOINTER)&_player_info.exp,            0, nullptr));
            TRYODBC(_hStmt, SQL_HANDLE_STMT,
                SQLBindCol(_hStmt, 12, SQL_C_SSHORT,  (SQLPOINTER)&_player_info.force,          0, nullptr));
            TRYODBC(_hStmt, SQL_HANDLE_STMT,
                SQLBindCol(_hStmt, 13, SQL_C_SSHORT,  (SQLPOINTER)&_player_info.dexterity,      0, nullptr));
            TRYODBC(_hStmt, SQL_HANDLE_STMT,
                SQLBindCol(_hStmt, 14, SQL_C_SSHORT,  (SQLPOINTER)&_player_info.health,         0, nullptr));
            TRYODBC(_hStmt, SQL_HANDLE_STMT,
                SQLBindCol(_hStmt, 15, SQL_C_SSHORT,  (SQLPOINTER)&_player_info.soul,           0, nullptr));
            TRYODBC(_hStmt, SQL_HANDLE_STMT,
                SQLBindCol(_hStmt, 16, SQL_C_SSHORT,  (SQLPOINTER)&_player_info.add_points,     0, nullptr));
            TRYODBC(_hStmt, SQL_HANDLE_STMT,
                SQLBindCol(_hStmt, 17, SQL_C_SSHORT,  (SQLPOINTER)&_player_info.life,           0, nullptr));
            TRYODBC(_hStmt, SQL_HANDLE_STMT,
                SQLBindCol(_hStmt, 18, SQL_C_SSHORT,  (SQLPOINTER)&_player_info.mana,           0, nullptr));
            TRYODBC(_hStmt, SQL_HANDLE_STMT,
                SQLBindCol(_hStmt, 19, SQL_C_TINYINT, (SQLPOINTER)&_player_info.profession,     0, nullptr));
            TRYODBC(_hStmt, SQL_HANDLE_STMT, 
                SQLBindCol(_hStmt, 20, SQL_C_SSHORT,  (SQLPOINTER)&_player_info.pk_points,      0, nullptr));
            TRYODBC(_hStmt, SQL_HANDLE_STMT,  
                SQLBindCol(_hStmt, 21, SQL_C_LONG,    (SQLPOINTER)&_player_info.virtue,         0, nullptr));
            TRYODBC(_hStmt, SQL_HANDLE_STMT,
                SQLBindCol(_hStmt, 22, SQL_C_TINYINT, (SQLPOINTER)&_player_info.nobility,       0, nullptr));
            TRYODBC(_hStmt, SQL_HANDLE_STMT,
                SQLBindCol(_hStmt, 23, SQL_C_TINYINT, (SQLPOINTER)&_player_info.rebirth,        0, nullptr));
            TRYODBC(_hStmt, SQL_HANDLE_STMT,
                SQLBindCol(_hStmt, 24, SQL_C_SSHORT,  (SQLPOINTER)&_player_info.syndicate_id,   0, nullptr));
            TRYODBC(_hStmt, SQL_HANDLE_STMT,
                SQLBindCol(_hStmt, 25, SQL_C_LONG,    (SQLPOINTER)&_player_info.record_map,     0, nullptr));
            TRYODBC(_hStmt, SQL_HANDLE_STMT,
                SQLBindCol(_hStmt, 26, SQL_C_SSHORT,  (SQLPOINTER)&_player_info.record_x,       0, nullptr));
            TRYODBC(_hStmt, SQL_HANDLE_STMT,
                SQLBindCol(_hStmt, 27, SQL_C_SSHORT,  (SQLPOINTER)&_player_info.record_y,       0, nullptr));
            TRYODBC(_hStmt, SQL_HANDLE_STMT,  
                SQLBindCol(_hStmt, 28, SQL_C_SBIGINT, (SQLPOINTER)&_player_info.last_login,     0, nullptr));

            return SQLBindParameter(_hStmt,
                PARAM_ONE,
                SQL_PARAM_INPUT, C_DWORD, SQL_INTEGER,
                NULL, NULL,
                (SQLPOINTER)&_player_info.identity,
                NULL, NULL);

        bailout:
            return SQL_ERROR;         
        }

        std::optional<PlayerInfo> GetPlayerInfo::execute()
        {
            SQLRETURN ret = SQL_SUCCESS;
            ret = SQLExecute(_hStmt);
            if (!SQL_SUCCEEDED(ret)) {
                return std::nullopt;
            }

            ret = SQLFetch(_hStmt);
            if (SQL_NO_DATA == ret) {
                _player_info.identity = PLAYER_STATS_NON_EXISTING;
                return { PlayerInfo(_player_info) };
            }

            if (!SQL_SUCCEEDED(ret)) {
                // handle error
                return std::nullopt;
            }

            return { PlayerInfo(_player_info) };
        }
    }
}
