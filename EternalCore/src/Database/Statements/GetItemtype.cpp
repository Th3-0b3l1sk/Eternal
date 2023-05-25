#include "Database/Statements/GetItemtype.h"

namespace Eternal
{
    namespace Database
    {
        GetItemtype::GetItemtype(uint32_t item_id)
            : IStatement(StatementID::GET_ITEMTYPE)
        {
            _info = std::make_unique<uint8_t[]>(sizeof(Info));
            auto info = (Info*)_info.get();
            info->id = item_id;
        }
        GetItemtype::GetItemtype(std::vector<uint32_t> item_ids)
            : IStatement(StatementID::GET_ITEMTYPE)
        {
            _info = std::make_unique<uint8_t[]>(sizeof(Info));
            _item_ids = item_ids;
        }
        SQLRETURN GetItemtype::bind()
        {
            auto info = (Info*)_info.get();
            if(_item_ids.empty())
                return SQLBindParameter(_hStatement,
                    PARAM_ONE,
                    SQL_PARAM_INPUT, C_DWORD, SQL_INTEGER,
                    NULL, NULL,
                    (SQLPOINTER)&info->id,
                    NULL, NULL);
            
            SQLRETURN result = SQL_SUCCESS;
            for (int i{}; i < _item_ids.size(); i++)
                result |= SQLBindParameter(_hStatement,
                      i + 1,
                      SQL_PARAM_INPUT, C_DWORD, SQL_INTEGER,
                      NULL, NULL,
                      (SQLPOINTER)&_item_ids[i],
                      NULL, NULL);

            return result;
        }

        std::vector<std::unique_ptr<uint8_t[]>> GetItemtype::fetch()
        {
            SQLRETURN rc = SQL_NO_DATA;
            auto data = std::vector<std::unique_ptr<uint8_t[]>>{};
            Info* ptr = (Info*)new uint8_t[sizeof(Info)]{};
            
            TRYODBC(_hStatement, SQL_HANDLE_STMT, 
                SQLBindCol(_hStatement, 1,   SQL_C_ULONG,     &ptr->id,              0, nullptr));
            TRYODBC(_hStatement, SQL_HANDLE_STMT,             
                SQLBindCol(_hStatement, 2,   SQL_C_CHAR,      ptr->name,  MAX_NAME_LEN, nullptr));
            TRYODBC(_hStatement, SQL_HANDLE_STMT,             
                SQLBindCol(_hStatement, 3,   SQL_C_UTINYINT,  &ptr->req_profession,  0, nullptr));
            TRYODBC(_hStatement, SQL_HANDLE_STMT,                                              
                SQLBindCol(_hStatement, 4,   SQL_C_UTINYINT,  &ptr->req_weaponskill, 0, nullptr));
            TRYODBC(_hStatement, SQL_HANDLE_STMT,                                              
                SQLBindCol(_hStatement, 5,   SQL_C_UTINYINT,  &ptr->req_level,       0, nullptr));
            TRYODBC(_hStatement, SQL_HANDLE_STMT,                                                                           
                SQLBindCol(_hStatement, 6,   SQL_C_UTINYINT,  &ptr->req_sex,         0, nullptr));
            TRYODBC(_hStatement, SQL_HANDLE_STMT,                                                                                     
                SQLBindCol(_hStatement, 7,   SQL_C_USHORT,    &ptr->req_force,       0, nullptr));
            TRYODBC(_hStatement, SQL_HANDLE_STMT,                                                                            
                SQLBindCol(_hStatement, 8,   SQL_C_USHORT,    &ptr->req_speed,       0, nullptr));
            TRYODBC(_hStatement, SQL_HANDLE_STMT,                                                                            
                SQLBindCol(_hStatement, 9,   SQL_C_USHORT,    &ptr->req_health,      0, nullptr));
            TRYODBC(_hStatement, SQL_HANDLE_STMT,                                                                            
                SQLBindCol(_hStatement, 10,  SQL_C_USHORT,    &ptr->req_soul,        0, nullptr));
            TRYODBC(_hStatement, SQL_HANDLE_STMT,                                                                                     
                SQLBindCol(_hStatement, 11,  SQL_C_UTINYINT,  &ptr->monopoly,        0, nullptr));
            TRYODBC(_hStatement, SQL_HANDLE_STMT,                                                                            
                SQLBindCol(_hStatement, 12,  SQL_C_USHORT,    &ptr->weight,          0, nullptr));
            TRYODBC(_hStatement, SQL_HANDLE_STMT,                                                                            
                SQLBindCol(_hStatement, 13,  SQL_C_ULONG,     &ptr->price,           0, nullptr));
            TRYODBC(_hStatement, SQL_HANDLE_STMT,                                                                            
                SQLBindCol(_hStatement, 14,  SQL_C_ULONG,     &ptr->task,            0, nullptr));
            TRYODBC(_hStatement, SQL_HANDLE_STMT,                                                                            
                SQLBindCol(_hStatement, 15,  SQL_C_USHORT,    &ptr->atk_max,         0, nullptr));
            TRYODBC(_hStatement, SQL_HANDLE_STMT,
                SQLBindCol(_hStatement, 16,  SQL_C_USHORT,    &ptr->atk_min,         0, nullptr));
            TRYODBC(_hStatement, SQL_HANDLE_STMT,                                                                            
                SQLBindCol(_hStatement, 17,  SQL_C_USHORT,    &ptr->defense,         0, nullptr));
            TRYODBC(_hStatement, SQL_HANDLE_STMT,                                                                            
                SQLBindCol(_hStatement, 18,  SQL_C_USHORT,    &ptr->dexterity,       0, nullptr));
            TRYODBC(_hStatement, SQL_HANDLE_STMT,                                                                            
                SQLBindCol(_hStatement, 19,  SQL_C_USHORT,    &ptr->dodge,           0, nullptr));
            TRYODBC(_hStatement, SQL_HANDLE_STMT,                                                                            
                SQLBindCol(_hStatement, 20,  SQL_C_USHORT,    &ptr->life,            0, nullptr));
            TRYODBC(_hStatement, SQL_HANDLE_STMT,                                                                            
                SQLBindCol(_hStatement, 21,  SQL_C_USHORT,    &ptr->mana,            0, nullptr));
            TRYODBC(_hStatement, SQL_HANDLE_STMT,                                                                            
                SQLBindCol(_hStatement, 22,  SQL_C_ULONG,     &ptr->dura,            0, nullptr));
            TRYODBC(_hStatement, SQL_HANDLE_STMT,                                               
                SQLBindCol(_hStatement, 23,  SQL_C_ULONG,     &ptr->dura_limit,      0, nullptr));
            TRYODBC(_hStatement, SQL_HANDLE_STMT,                                                                                     
                SQLBindCol(_hStatement, 24,  SQL_C_UTINYINT,  &ptr->status,          0, nullptr));
            TRYODBC(_hStatement, SQL_HANDLE_STMT,                                                                           
                SQLBindCol(_hStatement, 25,  SQL_C_UTINYINT,  &ptr->gem1,            0, nullptr));
            TRYODBC(_hStatement, SQL_HANDLE_STMT,                                                                           
                SQLBindCol(_hStatement, 26,  SQL_C_UTINYINT,  &ptr->gem2,            0, nullptr));
            TRYODBC(_hStatement, SQL_HANDLE_STMT,                                                                           
                SQLBindCol(_hStatement, 27,  SQL_C_UTINYINT,  &ptr->magic1,          0, nullptr));
            TRYODBC(_hStatement, SQL_HANDLE_STMT,                                                                           
                SQLBindCol(_hStatement, 28,  SQL_C_UTINYINT,  &ptr->magic2,          0, nullptr));
            TRYODBC(_hStatement, SQL_HANDLE_STMT,                                                                           
                SQLBindCol(_hStatement, 29,  SQL_C_UTINYINT,  &ptr->magic3,          0, nullptr));
            TRYODBC(_hStatement, SQL_HANDLE_STMT,                                                                                     
                SQLBindCol(_hStatement, 30,  SQL_C_USHORT,    &ptr->magic_atk,       0, nullptr));
            TRYODBC(_hStatement, SQL_HANDLE_STMT,                                                                            
                SQLBindCol(_hStatement, 31,  SQL_C_USHORT,    &ptr->magic_def,       0, nullptr));
            TRYODBC(_hStatement, SQL_HANDLE_STMT,                                                                            
                SQLBindCol(_hStatement, 32,  SQL_C_USHORT,    &ptr->atk_range,       0, nullptr));
            TRYODBC(_hStatement, SQL_HANDLE_STMT,                                                                            
                SQLBindCol(_hStatement, 33,  SQL_C_USHORT,    &ptr->atk_speed,       0, nullptr));
            TRYODBC(_hStatement, SQL_HANDLE_STMT,                                                                           
                SQLBindCol(_hStatement, 34,  SQL_C_UTINYINT,  &ptr->fray_mode,       0, nullptr));
            TRYODBC(_hStatement, SQL_HANDLE_STMT,                                                                           
                SQLBindCol(_hStatement, 35,  SQL_C_UTINYINT,  &ptr->repair_mode,     0, nullptr));
            TRYODBC(_hStatement, SQL_HANDLE_STMT,                                                                           
                SQLBindCol(_hStatement, 36,  SQL_C_UTINYINT,  &ptr->type_mask,       0, nullptr));
            TRYODBC(_hStatement, SQL_HANDLE_STMT,                                                                                     
                SQLBindCol(_hStatement, 37,  SQL_C_ULONG,     &ptr->cps_price,       0, nullptr));
                
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

        void GetItemtype::hook_stmt(std::string& stmt)
        {
            auto info = (Info*)_info.get();
            // default stmt: where [_id]=?
            if (_item_ids.empty() && info->id != 0)
                return;
            
            // load all items
            if (info->id == 0) {
                static const char* segment = " OR 1=1";
                std::string new_statement = stmt + segment;
                stmt = std::move(new_statement);
                return;
            }

            // load a subset of the items
            auto remaining = _item_ids.size() - 1;
            static const char* segment = " OR [_id]=?";
            std::string new_statement = stmt;
            for (int i{}; i < remaining; i++)
                new_statement += segment;

            stmt = std::move(new_statement);
        }
    }
}
