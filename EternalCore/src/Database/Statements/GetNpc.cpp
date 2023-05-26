#include "Database/Statements/GetNpc.h"

namespace Eternal
{
    namespace Database
    {
        GetNpc::GetNpc(uint32_t npc_id)
            : IStatement(StatementID::GET_NPC)
        {
            _info = std::make_unique<uint8_t[]>(sizeof Info);
            auto info = (Info*)_info.get();
            info->id = npc_id;
        }

        SQLRETURN GetNpc::bind()
        {
            auto info = (Info*)_info.get();
            return SQLBindParameter(_hStatement,
                PARAM_ONE,
                SQL_PARAM_INPUT, C_DWORD, SQL_INTEGER,
                NULL, NULL,
                (SQLPOINTER)&info->id,
                NULL, NULL);
        }

        std::vector<std::unique_ptr<uint8_t[]>> GetNpc::fetch()
        {
            SQLRETURN rc = SQL_NO_DATA;
            auto data = std::vector<std::unique_ptr<uint8_t[]>>{};
            Info* ptr = (Info*)new uint8_t[sizeof(Info)]{};

            TRYODBC(_hStatement, SQL_HANDLE_STMT,
                SQLBindCol(_hStatement,  1, SQL_C_ULONG,    &ptr->id,               0, nullptr));
            TRYODBC(_hStatement, SQL_HANDLE_STMT,
                SQLBindCol(_hStatement,  2, SQL_CHAR,       &ptr->name,  MAX_NAME_LEN, nullptr));
            TRYODBC(_hStatement, SQL_HANDLE_STMT,
                SQLBindCol(_hStatement,  3, SQL_C_UTINYINT, &ptr->type,             0, nullptr));
            TRYODBC(_hStatement, SQL_HANDLE_STMT,                                   
                SQLBindCol(_hStatement,  4, SQL_C_ULONG,    &ptr->lookface,         0, nullptr));
            TRYODBC(_hStatement, SQL_HANDLE_STMT,                                   
                SQLBindCol(_hStatement,  5, SQL_C_ULONG,    &ptr->map_id,           0, nullptr));
            TRYODBC(_hStatement, SQL_HANDLE_STMT,                                   
                SQLBindCol(_hStatement,  6, SQL_C_USHORT,   &ptr->cell_x,           0, nullptr));
            TRYODBC(_hStatement, SQL_HANDLE_STMT,                                   
                SQLBindCol(_hStatement,  7, SQL_C_USHORT,   &ptr->cell_y,           0, nullptr));
            TRYODBC(_hStatement, SQL_HANDLE_STMT,                                   
                SQLBindCol(_hStatement,  8, SQL_C_USHORT,   &ptr->life,             0, nullptr));
            TRYODBC(_hStatement, SQL_HANDLE_STMT,                                   
                SQLBindCol(_hStatement,  9, SQL_C_USHORT,   &ptr->maxlife,          0, nullptr));
            TRYODBC(_hStatement, SQL_HANDLE_STMT,                                   
                SQLBindCol(_hStatement, 10, SQL_C_UTINYINT, &ptr->base,             0, nullptr));
            TRYODBC(_hStatement, SQL_HANDLE_STMT,                                   
                SQLBindCol(_hStatement, 11, SQL_C_UTINYINT, &ptr->sort,             0, nullptr));

            while (SQLFetch(_hStatement) == SQL_SUCCESS)
            {
                auto citem = std::make_unique<uint8_t[]>(sizeof(Info));
                memcpy_s(citem.get(), sizeof(Info), ptr, sizeof(Info));
                data.push_back(std::move(citem));
            }

            delete[] ptr;
            SQLCloseCursor(_hStatement);
            return std::move(data);

        bailout:
            delete[] ptr;
            SQLCloseCursor(_hStatement);
            return {};

        }

        void GetNpc::hook_stmt(std::string& stmt)
        {
            auto info = (Info*)_info.get();
            if (info->id != 0)
                return;
            
            // load all NPCs
            static const char* segment = " OR 1=1";
            std::string new_statement = stmt + segment;
            stmt = std::move(new_statement);
            return;
        }
    }
}
