#include "Database/Statements/GetUserItems.h"

namespace Eternal
{
    namespace Database
    {
        GetUserItems::GetUserItems(uint32_t user_id)
            : IStatement(StatementID::GET_USER_ITEMS)
        {
            _info = std::make_unique<uint8_t[]>(sizeof(Info));
            ((Info*)(_info.get()))->owner_id = user_id;
        }
        SQLRETURN GetUserItems::bind()
        {
            auto info = (Info*)_info.get();
            return SQLBindParameter(_hStatement,
                PARAM_ONE,
                SQL_PARAM_INPUT, C_DWORD, SQL_INTEGER,
                NULL, NULL,
                (SQLPOINTER)&info->owner_id,
                NULL, NULL);
        }
        std::vector<std::unique_ptr<uint8_t[]>> GetUserItems::fetch()
        {
            auto data = std::vector<std::unique_ptr<uint8_t[]>>{};
            Info* item = (Info*)new uint8_t[sizeof(Info)]{};
            
            TRYODBC(_hStatement, SQL_HANDLE_STMT, 
                SQLBindCol(_hStatement, 1, SQL_C_LONG,     &item->id,         0, NULL));
            TRYODBC(_hStatement, SQL_HANDLE_STMT, 
                SQLBindCol(_hStatement, 2, SQL_C_LONG,     &item->type,       0, NULL));
            TRYODBC(_hStatement, SQL_HANDLE_STMT, 
                SQLBindCol(_hStatement, 3, SQL_C_LONG,     &item->owner_id,   0, NULL));
            TRYODBC(_hStatement, SQL_HANDLE_STMT, 
                SQLBindCol(_hStatement, 4, SQL_C_LONG,     &item->player_id,  0, NULL));
            TRYODBC(_hStatement, SQL_HANDLE_STMT, 
                SQLBindCol(_hStatement, 5, SQL_C_SHORT,    &item->dura,       0, NULL));
            TRYODBC(_hStatement, SQL_HANDLE_STMT, 
                SQLBindCol(_hStatement, 6, SQL_C_SHORT,    &item->dura_limit, 0, NULL));
            TRYODBC(_hStatement, SQL_HANDLE_STMT, 
                SQLBindCol(_hStatement, 7, SQL_C_TINYINT,  &item->ident,      0, NULL));
            TRYODBC(_hStatement, SQL_HANDLE_STMT, 
                SQLBindCol(_hStatement, 8, SQL_C_TINYINT,  &item->position,   0, NULL));
            TRYODBC(_hStatement, SQL_HANDLE_STMT, 
                SQLBindCol(_hStatement, 9, SQL_C_TINYINT,  &item->gem1,       0, NULL));
            TRYODBC(_hStatement, SQL_HANDLE_STMT, 
                SQLBindCol(_hStatement, 10, SQL_C_TINYINT, &item->gem2,       0, NULL));
            TRYODBC(_hStatement, SQL_HANDLE_STMT, 
                SQLBindCol(_hStatement, 11, SQL_C_TINYINT, &item->magic1,     0, NULL));
            TRYODBC(_hStatement, SQL_HANDLE_STMT, 
                SQLBindCol(_hStatement, 12, SQL_C_TINYINT, &item->magic2,     0, NULL));
            TRYODBC(_hStatement, SQL_HANDLE_STMT, 
                SQLBindCol(_hStatement, 13, SQL_C_TINYINT, &item->magic3,     0, NULL));
            TRYODBC(_hStatement, SQL_HANDLE_STMT, 
                SQLBindCol(_hStatement, 14, SQL_C_LONG,    &item->restrain,   0, NULL));
            TRYODBC(_hStatement, SQL_HANDLE_STMT, 
                SQLBindCol(_hStatement, 15, SQL_C_TINYINT, &item->bless,      0, NULL));
            TRYODBC(_hStatement, SQL_HANDLE_STMT, 
                SQLBindCol(_hStatement, 16, SQL_C_TINYINT, &item->enchant,    0, NULL));
            TRYODBC(_hStatement, SQL_HANDLE_STMT, 
                SQLBindCol(_hStatement, 17, SQL_C_TINYINT, &item->sus,        0, NULL));
            TRYODBC(_hStatement, SQL_HANDLE_STMT, 
                SQLBindCol(_hStatement, 18, SQL_C_TINYINT, &item->locked,     0, NULL));
            TRYODBC(_hStatement, SQL_HANDLE_STMT, 
                SQLBindCol(_hStatement, 19, SQL_C_TINYINT, &item->color,      0, NULL));

            while(SQLFetch(_hStatement) == SQL_SUCCESS)
            { 
                auto citem = std::make_unique<uint8_t[]>(sizeof(Info));
                memcpy_s(citem.get(), sizeof(Info), item, sizeof(Info));
                data.push_back(std::move(citem));
            }

            delete[] item;
            SQLCloseCursor(_hStatement);
            return std::move(data);

        bailout:
            delete[] item;
            SQLCloseCursor(_hStatement);
            return {};
        }
    }
}
