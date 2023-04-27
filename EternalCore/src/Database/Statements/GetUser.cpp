#include "Database/Statements/GetUser.h"

namespace Eternal
{
    namespace Database
    {
        GetUser::GetUser(uint32_t id)
            : IStatement(StatementID::GET_USER)
        {
            _info = std::make_unique<uint8_t[]>(sizeof(Info));
            ((Info*)(_info.get()))->identity = id;    // beautiful :)
        }
        SQLRETURN GetUser::bind()
        {
            auto info = (Info*)_info.get();
            return SQLBindParameter(_hStatement,
                PARAM_ONE,
                SQL_PARAM_INPUT, C_DWORD, SQL_INTEGER,
                NULL, NULL,
                (SQLPOINTER)&info->identity,
                NULL, NULL);
        }
        
        std::vector<std::unique_ptr<uint8_t[]>> GetUser::fetch()
        {
            SQLRETURN rc = SQL_NO_DATA;
            auto data = std::vector<std::unique_ptr<uint8_t[]>>{};
            Info* ptr = (Info*)new uint8_t[sizeof(Info)]{};

            /* programming is FUN */
            TRYODBC(_hStatement, SQL_HANDLE_STMT,
                SQLBindCol(_hStatement, 1,  SQL_C_LONG,    &ptr->id,             0, nullptr));
            TRYODBC(_hStatement, SQL_HANDLE_STMT,
                SQLBindCol(_hStatement, 2,  SQL_C_LONG,    &ptr->identity,       0, nullptr));
            TRYODBC(_hStatement, SQL_HANDLE_STMT,
                SQLBindCol(_hStatement, 3,  SQL_C_CHAR,    ptr->name, MAX_NAME_LEN, nullptr));
            TRYODBC(_hStatement, SQL_HANDLE_STMT,
                SQLBindCol(_hStatement, 4,  SQL_C_CHAR,    ptr->mate, MAX_NAME_LEN, nullptr));
            TRYODBC(_hStatement, SQL_HANDLE_STMT,
                SQLBindCol(_hStatement, 5,  SQL_C_LONG,    &ptr->lookface,       0, nullptr));
            TRYODBC(_hStatement, SQL_HANDLE_STMT,
                SQLBindCol(_hStatement, 6,  SQL_C_SSHORT,  &ptr->hair,           0, nullptr));
            TRYODBC(_hStatement, SQL_HANDLE_STMT,
                SQLBindCol(_hStatement, 7,  SQL_C_LONG,    &ptr->money,          0, nullptr));
            TRYODBC(_hStatement, SQL_HANDLE_STMT,
                SQLBindCol(_hStatement, 8,  SQL_C_LONG,    &ptr->money_saved,    0, nullptr));
            TRYODBC(_hStatement, SQL_HANDLE_STMT,
                SQLBindCol(_hStatement, 9,  SQL_C_LONG,    &ptr->cps,            0, nullptr));
            TRYODBC(_hStatement, SQL_HANDLE_STMT,
                SQLBindCol(_hStatement, 10, SQL_C_TINYINT, &ptr->level,          0, nullptr));
            TRYODBC(_hStatement, SQL_HANDLE_STMT,
                SQLBindCol(_hStatement, 11, SQL_C_SBIGINT, &ptr->exp,            0, nullptr));
            TRYODBC(_hStatement, SQL_HANDLE_STMT,
                SQLBindCol(_hStatement, 12, SQL_C_SSHORT,  &ptr->force,          0, nullptr));
            TRYODBC(_hStatement, SQL_HANDLE_STMT,
                SQLBindCol(_hStatement, 13, SQL_C_SSHORT,  &ptr->dexterity,      0, nullptr));
            TRYODBC(_hStatement, SQL_HANDLE_STMT,
                SQLBindCol(_hStatement, 14, SQL_C_SSHORT,  &ptr->health,         0, nullptr));
            TRYODBC(_hStatement, SQL_HANDLE_STMT,
                SQLBindCol(_hStatement, 15, SQL_C_SSHORT,  &ptr->soul,           0, nullptr));
            TRYODBC(_hStatement, SQL_HANDLE_STMT,
                SQLBindCol(_hStatement, 16, SQL_C_SSHORT,  &ptr->add_points,     0, nullptr));
            TRYODBC(_hStatement, SQL_HANDLE_STMT,
                SQLBindCol(_hStatement, 17, SQL_C_SSHORT,  &ptr->life,           0, nullptr));
            TRYODBC(_hStatement, SQL_HANDLE_STMT,
                SQLBindCol(_hStatement, 18, SQL_C_SSHORT,  &ptr->mana,           0, nullptr));
            TRYODBC(_hStatement, SQL_HANDLE_STMT,
                SQLBindCol(_hStatement, 19, SQL_C_TINYINT, &ptr->profession,     0, nullptr));
            TRYODBC(_hStatement, SQL_HANDLE_STMT, 
                SQLBindCol(_hStatement, 20, SQL_C_SSHORT,  &ptr->pk,             0, nullptr));
            TRYODBC(_hStatement, SQL_HANDLE_STMT,  
                SQLBindCol(_hStatement, 21, SQL_C_LONG,    &ptr->virtue,         0, nullptr));
            TRYODBC(_hStatement, SQL_HANDLE_STMT,
                SQLBindCol(_hStatement, 22, SQL_C_TINYINT, &ptr->nobility,       0, nullptr));
            TRYODBC(_hStatement, SQL_HANDLE_STMT,
                SQLBindCol(_hStatement, 23, SQL_C_TINYINT, &ptr->rebirth,        0, nullptr));
            TRYODBC(_hStatement, SQL_HANDLE_STMT,
                SQLBindCol(_hStatement, 24, SQL_C_SSHORT,  &ptr->syndicate_id,   0, nullptr));
            TRYODBC(_hStatement, SQL_HANDLE_STMT,
                SQLBindCol(_hStatement, 25, SQL_C_LONG,    &ptr->record_map,     0, nullptr));
            TRYODBC(_hStatement, SQL_HANDLE_STMT,
                SQLBindCol(_hStatement, 26, SQL_C_SSHORT,  &ptr->record_x,       0, nullptr));
            TRYODBC(_hStatement, SQL_HANDLE_STMT,
                SQLBindCol(_hStatement, 27, SQL_C_SSHORT,  &ptr->record_y,       0, nullptr));
            TRYODBC(_hStatement, SQL_HANDLE_STMT,  
                SQLBindCol(_hStatement, 28, SQL_C_SBIGINT, &ptr->last_login,     0, nullptr));


            rc = SQLFetch(_hStatement);
            if (rc == SQL_NO_DATA)
                return {};
            
            data.emplace_back((uint8_t*)ptr);
            return std::move(data);
        bailout:
            SQLCloseCursor(_hStatement);
            return {};
        }
    }
}
