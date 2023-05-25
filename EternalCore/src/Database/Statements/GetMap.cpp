#include "Database/Statements/GetMap.h"

namespace Eternal
{
    namespace Database
    {
        GetMap::GetMap()
            : IStatement(Eternal::Database::StatementID::GET_MAP)
        {
            _info = std::make_unique<uint8_t[]>(sizeof(Info));
        }

        GetMap::GetMap(uint16_t doc_id)
            : IStatement(Eternal::Database::StatementID::GET_MAP)
        {
            _info = std::make_unique<uint8_t[]>(sizeof(Info));
            auto info = (Info*)_info.get();
            info->_doc_id = doc_id;
        }

        SQLRETURN GetMap::bind()
        {
            auto info = (Info*)_info.get();
            return SQLBindParameter(_hStatement,
                PARAM_ONE,
                SQL_PARAM_INPUT, C_DWORD, SQL_INTEGER,
                NULL, NULL,
                (SQLPOINTER)&info->_doc_id,
                NULL, NULL);
        }

        std::vector<std::unique_ptr<uint8_t[]>> GetMap::fetch()
        {

            SQLRETURN rc = SQL_NO_DATA;
            auto data = std::vector<std::unique_ptr<uint8_t[]>>{};
            Info* info = (Info*)new uint8_t[sizeof(Info)]{};

            info->_owner_id = 0;
            TRYODBC(_hStatement, SQL_HANDLE_STMT,
                SQLBindCol(_hStatement, 1,  SQL_C_SHORT, &info->_uid,           0, nullptr));
            TRYODBC(_hStatement, SQL_HANDLE_STMT,
                SQLBindCol(_hStatement, 4,  SQL_C_SHORT, &info->_doc_id,        0, nullptr));
            TRYODBC(_hStatement, SQL_HANDLE_STMT, 
                SQLBindCol(_hStatement, 5,  SQL_C_ULONG, &info->_type,          0, nullptr));
            TRYODBC(_hStatement, SQL_HANDLE_STMT,
                SQLBindCol(_hStatement, 6,  SQL_C_ULONG, &info->_weather,       0, nullptr));
            TRYODBC(_hStatement, SQL_HANDLE_STMT,
                SQLBindCol(_hStatement, 9,  SQL_C_SHORT, &info->_portal_x,      0, nullptr));
            TRYODBC(_hStatement, SQL_HANDLE_STMT,
                SQLBindCol(_hStatement, 10, SQL_C_SHORT, &info->_portal_y,      0, nullptr));
            TRYODBC(_hStatement, SQL_HANDLE_STMT,
                SQLBindCol(_hStatement, 11, SQL_C_ULONG, &info->_reborn_map,    0, nullptr));
            TRYODBC(_hStatement, SQL_HANDLE_STMT,
                SQLBindCol(_hStatement, 12, SQL_C_ULONG, &info->_reborn_portal, 0, nullptr));
            TRYODBC(_hStatement, SQL_HANDLE_STMT,
                SQLBindCol(_hStatement, 13, SQL_C_LONG, &info->_light,          0, nullptr));

            while (SQLFetch(_hStatement) == SQL_SUCCESS)
            {
                auto map = std::make_unique<uint8_t[]>(sizeof(Info));
                memcpy_s(map.get(), sizeof(Info), info, sizeof(Info));
                data.push_back(std::move(map));
            }

            delete[] info;
            SQLCloseCursor(_hStatement);
            return std::move(data);

        bailout:
            delete[] info;
            SQLCloseCursor(_hStatement);
            return {};
        }

        void GetMap::hook_stmt(std::string& stmt)
        {
            auto info = (Info*)_info.get();
            if (info->_doc_id != 0)
                return;

            static const char* segment = " OR 1=1;"; // Hacky!
            std::string new_statement = stmt + segment;
            stmt = std::move(new_statement);
        }
    }
}
