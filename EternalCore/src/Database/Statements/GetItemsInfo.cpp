#include "Database/Statements/GetItemsInfo.h"

namespace Eternal
{
    namespace Database
    {
        GetItemsInfo::GetItemsInfo(SQLHANDLE hCon)

        {
            SQLAllocHandle(SQL_HANDLE_STMT, hCon, &_hStmt);
            _item_info.name.resize(MAX_NAME_LEN);
        }

        SQLRETURN GetItemsInfo::bind()
        {
            // prepare the statement
            // get_items_info()
            static std::string stmt = "EXEC get_items_info";
            TRYODBC(_hStmt, SQL_HANDLE_STMT,
                SQLPrepare(_hStmt, (SQLCHAR*)stmt.c_str(), stmt.size()));

            TRYODBC(_hStmt, SQL_HANDLE_STMT, 
                SQLBindCol(_hStmt, 1,   SQL_C_ULONG,     &_item_info.id,              0, nullptr));
            TRYODBC(_hStmt, SQL_HANDLE_STMT,             
                SQLBindCol(_hStmt, 2,   SQL_C_CHAR,      (SQLPOINTER)_item_info.name.c_str(), _item_info.name.size(), nullptr));
            TRYODBC(_hStmt, SQL_HANDLE_STMT,             
                SQLBindCol(_hStmt, 3,   SQL_C_UTINYINT,  &_item_info.req_profession,  0, nullptr));
            TRYODBC(_hStmt, SQL_HANDLE_STMT,                                              
                SQLBindCol(_hStmt, 4,   SQL_C_UTINYINT,  &_item_info.req_weaponskill, 0, nullptr));
            TRYODBC(_hStmt, SQL_HANDLE_STMT,                                              
                SQLBindCol(_hStmt, 5,   SQL_C_UTINYINT,  &_item_info.req_level,       0, nullptr));
            TRYODBC(_hStmt, SQL_HANDLE_STMT,                                                                           
                SQLBindCol(_hStmt, 6,   SQL_C_UTINYINT,  &_item_info.req_sex,         0, nullptr));
            TRYODBC(_hStmt, SQL_HANDLE_STMT,                                                                                     
                SQLBindCol(_hStmt, 7,   SQL_C_USHORT,    &_item_info.req_force,       0, nullptr));
            TRYODBC(_hStmt, SQL_HANDLE_STMT,                                                                            
                SQLBindCol(_hStmt, 8,   SQL_C_USHORT,    &_item_info.req_speed,       0, nullptr));
            TRYODBC(_hStmt, SQL_HANDLE_STMT,                                                                            
                SQLBindCol(_hStmt, 9,   SQL_C_USHORT,    &_item_info.req_health,      0, nullptr));
            TRYODBC(_hStmt, SQL_HANDLE_STMT,                                                                            
                SQLBindCol(_hStmt, 10,  SQL_C_USHORT,    &_item_info.req_soul,        0, nullptr));
            TRYODBC(_hStmt, SQL_HANDLE_STMT,                                                                                     
                SQLBindCol(_hStmt, 11,  SQL_C_UTINYINT,  &_item_info.monopoly,        0, nullptr));
            TRYODBC(_hStmt, SQL_HANDLE_STMT,                                                                            
                SQLBindCol(_hStmt, 12,  SQL_C_USHORT,    &_item_info.weight,          0, nullptr));
            TRYODBC(_hStmt, SQL_HANDLE_STMT,                                                                            
                SQLBindCol(_hStmt, 13,  SQL_C_ULONG,     &_item_info.price,           0, nullptr));
            TRYODBC(_hStmt, SQL_HANDLE_STMT,                                                                            
                SQLBindCol(_hStmt, 14,  SQL_C_ULONG,     &_item_info.task,            0, nullptr));
            TRYODBC(_hStmt, SQL_HANDLE_STMT,                                                                            
                SQLBindCol(_hStmt, 15,  SQL_C_USHORT,    &_item_info.atk_max,         0, nullptr));
            TRYODBC(_hStmt, SQL_HANDLE_STMT,
                SQLBindCol(_hStmt, 16,  SQL_C_USHORT,    &_item_info.atk_min,         0, nullptr));
            TRYODBC(_hStmt, SQL_HANDLE_STMT,                                                                            
                SQLBindCol(_hStmt, 17,  SQL_C_USHORT,    &_item_info.defense,         0, nullptr));
            TRYODBC(_hStmt, SQL_HANDLE_STMT,                                                                            
                SQLBindCol(_hStmt, 18,  SQL_C_USHORT,    &_item_info.dexterity,       0, nullptr));
            TRYODBC(_hStmt, SQL_HANDLE_STMT,                                                                            
                SQLBindCol(_hStmt, 19,  SQL_C_USHORT,    &_item_info.dodge,           0, nullptr));
            TRYODBC(_hStmt, SQL_HANDLE_STMT,                                                                            
                SQLBindCol(_hStmt, 20,  SQL_C_USHORT,    &_item_info.life,            0, nullptr));
            TRYODBC(_hStmt, SQL_HANDLE_STMT,                                                                            
                SQLBindCol(_hStmt, 21,  SQL_C_USHORT,    &_item_info.mana,            0, nullptr));
            TRYODBC(_hStmt, SQL_HANDLE_STMT,                                                                            
                SQLBindCol(_hStmt, 22,  SQL_C_ULONG,     &_item_info.dura,            0, nullptr));
            TRYODBC(_hStmt, SQL_HANDLE_STMT,                                               
                SQLBindCol(_hStmt, 23,  SQL_C_ULONG,     &_item_info.dura_limit,      0, nullptr));
            TRYODBC(_hStmt, SQL_HANDLE_STMT,                                                                                     
                SQLBindCol(_hStmt, 24,  SQL_C_UTINYINT,  &_item_info.status,          0, nullptr));
            TRYODBC(_hStmt, SQL_HANDLE_STMT,                                                                           
                SQLBindCol(_hStmt, 25,  SQL_C_UTINYINT,  &_item_info.gem1,            0, nullptr));
            TRYODBC(_hStmt, SQL_HANDLE_STMT,                                                                           
                SQLBindCol(_hStmt, 26,  SQL_C_UTINYINT,  &_item_info.gem2,            0, nullptr));
            TRYODBC(_hStmt, SQL_HANDLE_STMT,                                                                           
                SQLBindCol(_hStmt, 27,  SQL_C_UTINYINT,  &_item_info.magic1,          0, nullptr));
            TRYODBC(_hStmt, SQL_HANDLE_STMT,                                                                           
                SQLBindCol(_hStmt, 28,  SQL_C_UTINYINT,  &_item_info.magic2,          0, nullptr));
            TRYODBC(_hStmt, SQL_HANDLE_STMT,                                                                           
                SQLBindCol(_hStmt, 29,  SQL_C_UTINYINT,  &_item_info.magic3,          0, nullptr));
            TRYODBC(_hStmt, SQL_HANDLE_STMT,                                                                                     
                SQLBindCol(_hStmt, 30,  SQL_C_USHORT,    &_item_info.magic_atk,       0, nullptr));
            TRYODBC(_hStmt, SQL_HANDLE_STMT,                                                                            
                SQLBindCol(_hStmt, 31,  SQL_C_USHORT,    &_item_info.magic_def,       0, nullptr));
            TRYODBC(_hStmt, SQL_HANDLE_STMT,                                                                            
                SQLBindCol(_hStmt, 32,  SQL_C_USHORT,    &_item_info.atk_range,       0, nullptr));
            TRYODBC(_hStmt, SQL_HANDLE_STMT,                                                                            
                SQLBindCol(_hStmt, 33,  SQL_C_USHORT,    &_item_info.atk_speed,       0, nullptr));
            TRYODBC(_hStmt, SQL_HANDLE_STMT,                                                                           
                SQLBindCol(_hStmt, 34,  SQL_C_UTINYINT,  &_item_info.fray_mode,       0, nullptr));
            TRYODBC(_hStmt, SQL_HANDLE_STMT,                                                                           
                SQLBindCol(_hStmt, 35,  SQL_C_UTINYINT,  &_item_info.repair_mode,     0, nullptr));
            TRYODBC(_hStmt, SQL_HANDLE_STMT,                                                                           
                SQLBindCol(_hStmt, 36,  SQL_C_UTINYINT,  &_item_info.type_mask,       0, nullptr));
            TRYODBC(_hStmt, SQL_HANDLE_STMT,                                                                                     
                SQLBindCol(_hStmt, 37,  SQL_C_ULONG,     &_item_info.cps_price,       0, nullptr));

            // No parameters to bind, fetching it all

            return SQL_SUCCESS;
        bailout:
            return SQL_ERROR;
        }

        std::optional<std::vector<ItemInfo>> GetItemsInfo::execute()
        {
            SQLRETURN res = SQL_SUCCESS;
            res = SQLExecute(_hStmt);
            if (!SQL_SUCCEEDED(res)) {
                // TODO: handle error
                return std::nullopt;
            }

            res = SQLFetch(_hStmt);
            if (SQL_NO_DATA == res) {
                // failed to load items info
                return std::nullopt;
            }

            std::vector<ItemInfo> items;
            do 
            {
                items.push_back(_item_info);
            } while (SQL_SUCCESS == SQLFetch(_hStmt));

            SQLCloseCursor(_hStmt);
            return std::optional<std::vector<ItemInfo>>(std::move(items));
        }
    }
}
