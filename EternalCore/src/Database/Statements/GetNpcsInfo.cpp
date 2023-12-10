#include "Database/Statements/GetNpcsInfo.h"

namespace Eternal
{
    namespace Database
    {
        GetNpcsInfo::GetNpcsInfo(SQLHANDLE hCon)
        {
            SQLAllocHandle(SQL_HANDLE_STMT, hCon, &_hStmt);
            _npc_info.name.resize(MAX_NAME_LEN);
        }

        SQLRETURN GetNpcsInfo::bind()
        {
            // prepare the statement
            // get_npcs_info();
            static std::string stmt = "EXEC get_npcs_info";
            TRYODBC(_hStmt, SQL_HANDLE_STMT,
                SQLPrepare(_hStmt, (SQLCHAR*)stmt.c_str(), stmt.size()));

            // bind params
            
            TRYODBC(_hStmt, SQL_HANDLE_STMT,
                SQLBindCol(_hStmt,  1, SQL_C_ULONG,    &_npc_info.id,               0, nullptr));
            TRYODBC(_hStmt, SQL_HANDLE_STMT,
                SQLBindCol(_hStmt,  2, SQL_CHAR,       (SQLPOINTER)_npc_info.name.c_str(), _npc_info.name.size(), nullptr));
            TRYODBC(_hStmt, SQL_HANDLE_STMT,
                SQLBindCol(_hStmt,  3, SQL_C_UTINYINT, &_npc_info.type,             0, nullptr));
            TRYODBC(_hStmt, SQL_HANDLE_STMT,                                   
                SQLBindCol(_hStmt,  4, SQL_C_ULONG,    &_npc_info.lookface,         0, nullptr));
            TRYODBC(_hStmt, SQL_HANDLE_STMT,                                   
                SQLBindCol(_hStmt,  5, SQL_C_ULONG,    &_npc_info.map_id,           0, nullptr));
            TRYODBC(_hStmt, SQL_HANDLE_STMT,                                   
                SQLBindCol(_hStmt,  6, SQL_C_USHORT,   &_npc_info.cell_x,           0, nullptr));
            TRYODBC(_hStmt, SQL_HANDLE_STMT,                                   
                SQLBindCol(_hStmt,  7, SQL_C_USHORT,   &_npc_info.cell_y,           0, nullptr));
            TRYODBC(_hStmt, SQL_HANDLE_STMT,                                   
                SQLBindCol(_hStmt,  8, SQL_C_USHORT,   &_npc_info.life,             0, nullptr));
            TRYODBC(_hStmt, SQL_HANDLE_STMT,                                   
                SQLBindCol(_hStmt,  9, SQL_C_USHORT,   &_npc_info.maxlife,          0, nullptr));
            TRYODBC(_hStmt, SQL_HANDLE_STMT,                                   
                SQLBindCol(_hStmt, 10, SQL_C_UTINYINT, &_npc_info.base,             0, nullptr));
            TRYODBC(_hStmt, SQL_HANDLE_STMT,                                   
                SQLBindCol(_hStmt, 11, SQL_C_UTINYINT, &_npc_info.sort,             0, nullptr));

            return SQL_SUCCESS;
        bailout: 
            return SQL_ERROR;
            
        }

        std::optional<std::vector<NpcInfo>> GetNpcsInfo::execute()
        {
            SQLRETURN res = SQL_SUCCESS;
            res = SQLExecute(_hStmt);
            if (!SQL_SUCCEEDED(res)) {
                // handle errors
                return std::nullopt;
            }

            res = SQLFetch(_hStmt);
            if (SQL_NO_DATA == res) {
                return std::nullopt;
            }

            std::vector<NpcInfo> npcs;

            do
            {
                npcs.push_back(_npc_info);
            } while (SQL_SUCCESS == SQLFetch(_hStmt));

            SQLCloseCursor(_hStmt);
            return std::optional<std::vector<NpcInfo>>(std::move(npcs));
        }
    }
}
