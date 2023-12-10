#include "Database/Statements/GetMapsInfo.h"

namespace Eternal
{
    namespace Database
    {
        GetMapsInfo::GetMapsInfo(SQLHANDLE hCon)
        {
            SQLAllocHandle(SQL_HANDLE_STMT, hCon, &_hStmt);
            _map_info.name.resize(MAX_NAME_LEN);
        }

        SQLRETURN GetMapsInfo::bind()
        {
            // prepare the statement
            // get_maps_info()
            static std::string stmt = "EXEC get_maps_info";

            // bind the columns
            TRYODBC(_hStmt, SQL_HANDLE_STMT,
                SQLBindCol(_hStmt, 1,  SQL_C_SHORT, &_map_info.uid,           0, nullptr));
            TRYODBC(_hStmt, SQL_HANDLE_STMT,
                SQLBindCol(_hStmt, 2,  SQL_C_CHAR, (SQLPOINTER)_map_info.name.c_str(), _map_info.name.size(), nullptr));
            TRYODBC(_hStmt, SQL_HANDLE_STMT,
                SQLBindCol(_hStmt, 4,  SQL_C_SHORT, &_map_info.doc_id,        0, nullptr));
            TRYODBC(_hStmt, SQL_HANDLE_STMT, 
                SQLBindCol(_hStmt, 5,  SQL_C_ULONG, &_map_info.type,          0, nullptr));
            TRYODBC(_hStmt, SQL_HANDLE_STMT,
                SQLBindCol(_hStmt, 6,  SQL_C_ULONG, &_map_info.weather,       0, nullptr));
            TRYODBC(_hStmt, SQL_HANDLE_STMT,
                SQLBindCol(_hStmt, 9,  SQL_C_SHORT, &_map_info.portal_x,      0, nullptr));
            TRYODBC(_hStmt, SQL_HANDLE_STMT,
                SQLBindCol(_hStmt, 10, SQL_C_SHORT, &_map_info.portal_y,      0, nullptr));
            TRYODBC(_hStmt, SQL_HANDLE_STMT,
                SQLBindCol(_hStmt, 11, SQL_C_ULONG, &_map_info.reborn_map,    0, nullptr));
            TRYODBC(_hStmt, SQL_HANDLE_STMT,
                SQLBindCol(_hStmt, 12, SQL_C_ULONG, &_map_info.reborn_portal, 0, nullptr));
            TRYODBC(_hStmt, SQL_HANDLE_STMT,
                SQLBindCol(_hStmt, 13, SQL_C_LONG, &_map_info.light,          0, nullptr));

            return SQLPrepare(_hStmt, (SQLCHAR*)stmt.c_str(), stmt.size());

            return SQL_SUCCESS;

        bailout:
            return SQL_ERROR;
        }

        std::optional<std::vector<MapInfo>> GetMapsInfo::execute()
        {
            SQLRETURN res = SQL_SUCCESS;
            res = SQLExecute(_hStmt);
            if (!SQL_SUCCEEDED(res)) {
                // handle error
                auto error_msg = ::get_error(_hStmt, SQL_HANDLE_STMT, res);
                std::cout << "[!] DB Error:GetMapsInfo!execute():" << error_msg << '\n';
                return std::nullopt;
            }

            res = SQLFetch(_hStmt);
            if (SQL_NO_DATA == res) {
                // failed to load map data
                return std::nullopt;
            }

            std::vector<MapInfo> maps;
            do
            {
                maps.push_back(_map_info);
            } while (SQL_SUCCESS == SQLFetch(_hStmt));
            

            SQLCloseCursor(_hStmt);
            return std::optional<std::vector<MapInfo>>(std::move(maps));
        }
    }
}
