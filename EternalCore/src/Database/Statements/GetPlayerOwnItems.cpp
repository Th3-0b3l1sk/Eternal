#include "Database/Statements/GetPlayerOwnItems.h"
#include "Database/Database.h"

namespace Eternal
{
    namespace Database
    {
        GetPlayerOwnItems::GetPlayerOwnItems(SQLHANDLE hCon)
            : _player_id{ 0 }, _hStmt{ NULL }, _item{ 0 }
        {
            SQLAllocHandle(SQL_HANDLE_STMT, hCon, &_hStmt);
        }

        SQLRETURN GetPlayerOwnItems::bind()
        {
            // prepare the statement
            // get_player_own_item(player_id)
            static std::string stmt = "EXEC get_player_own_items @player_id=?";
            TRYODBC(_hStmt, SQL_HANDLE_STMT,
                SQLPrepare(_hStmt, (SQLCHAR*)stmt.c_str(), stmt.size()));

            // Bind item id
            TRYODBC(_hStmt, SQL_HANDLE_STMT, 
                SQLBindCol(_hStmt, 1, SQL_INTEGER,    (SQLPOINTER)_item.id,          sizeof(_item.id),          nullptr));
            
            // Bind item type
            TRYODBC(_hStmt, SQL_HANDLE_STMT, 
                SQLBindCol(_hStmt, 2, SQL_INTEGER,    (SQLPOINTER)_item.type,        sizeof(_item.type),        nullptr));
            
            // Bind owner id
            TRYODBC(_hStmt, SQL_HANDLE_STMT, 
                SQLBindCol(_hStmt, 3, SQL_INTEGER,    (SQLPOINTER)_item.owner_id,    sizeof(_item.owner_id),    nullptr));
            
            // Bind player id < we already have it>
            
            // Bind dura
            TRYODBC(_hStmt, SQL_HANDLE_STMT, 
                SQLBindCol(_hStmt, 5, SQL_SMALLINT,   (SQLPOINTER)_item.dura,       sizeof(_item.dura),       nullptr));
            
            // Bind dura limit
            TRYODBC(_hStmt, SQL_HANDLE_STMT, 
                SQLBindCol(_hStmt, 6, SQL_SMALLINT,   (SQLPOINTER)_item.dura_limit, sizeof(_item.dura_limit), nullptr));
            
            // Bind ident
            TRYODBC(_hStmt, SQL_HANDLE_STMT, 
                SQLBindCol(_hStmt, 7, SQL_TINYINT,    (SQLPOINTER)_item.ident,      sizeof(_item.ident),      nullptr));
            
            // Bind position
            TRYODBC(_hStmt, SQL_HANDLE_STMT, 
                SQLBindCol(_hStmt, 8, SQL_TINYINT,    (SQLPOINTER)_item.position,   sizeof(_item.position),   nullptr));
            
            // Bind gem1
            TRYODBC(_hStmt, SQL_HANDLE_STMT, 
                SQLBindCol(_hStmt, 9, SQL_TINYINT,    (SQLPOINTER)_item.gem1,       sizeof(_item.gem1),       nullptr));
            
            // Bind gem2
            TRYODBC(_hStmt, SQL_HANDLE_STMT, 
                SQLBindCol(_hStmt, 10, SQL_TINYINT,   (SQLPOINTER)_item.gem2,       sizeof(_item.gem2),       nullptr));
            
            // Bind magic1
            TRYODBC(_hStmt, SQL_HANDLE_STMT, 
                SQLBindCol(_hStmt, 11, SQL_TINYINT,   (SQLPOINTER)_item.magic1,     sizeof(_item.magic1),     nullptr));
            
            // Bind magic2
            TRYODBC(_hStmt, SQL_HANDLE_STMT, 
                SQLBindCol(_hStmt, 12, SQL_TINYINT,   (SQLPOINTER)_item.magic2,     sizeof(_item.magic2),     nullptr));
            
            // Bind magic3
            TRYODBC(_hStmt, SQL_HANDLE_STMT, 
                SQLBindCol(_hStmt, 13, SQL_TINYINT,   (SQLPOINTER)_item.magic3,     sizeof(_item.magic3),     nullptr));
            
            // Bind restrain
            TRYODBC(_hStmt, SQL_HANDLE_STMT, 
                SQLBindCol(_hStmt, 14, SQL_INTEGER,   (SQLPOINTER)_item.restrain,   sizeof(_item.restrain),   nullptr));
            
            // Bind bless
            TRYODBC(_hStmt, SQL_HANDLE_STMT, 
                SQLBindCol(_hStmt, 15, SQL_TINYINT,   (SQLPOINTER)_item.bless,      sizeof(_item.bless),      nullptr));
            
            // Bind enchant
            TRYODBC(_hStmt, SQL_HANDLE_STMT, 
                SQLBindCol(_hStmt, 16, SQL_TINYINT,   (SQLPOINTER)_item.enchant,    sizeof(_item.enchant),    nullptr));
            
            // Bind is_sussy
            TRYODBC(_hStmt, SQL_HANDLE_STMT, 
                SQLBindCol(_hStmt, 17, SQL_TINYINT,   (SQLPOINTER)_item.is_sussy,   sizeof(_item.is_sussy),   nullptr));
            
            // Bind is_locked
            TRYODBC(_hStmt, SQL_HANDLE_STMT, 
                SQLBindCol(_hStmt, 18, SQL_TINYINT,   (SQLPOINTER)_item.is_locked,  sizeof(_item.is_locked),  nullptr));
            
            // Bind color
            TRYODBC(_hStmt, SQL_HANDLE_STMT, 
                SQLBindCol(_hStmt, 18, SQL_TINYINT,   (SQLPOINTER)_item.color,      sizeof(_item.color),      nullptr));


            return SQLBindParameter(_hStmt,
                    PARAM_ONE,
                    SQL_PARAM_INPUT, C_DWORD, SQL_INTEGER,
                    NULL, NULL,
                    (SQLPOINTER)&_player_id,
                    NULL, NULL);

        bailout:
            return SQL_ERROR;
        }

        // needs to be an optional
        std::optional<std::vector<PlayerOwnItem>> GetPlayerOwnItems::execute()
        {
            SQLRETURN ret = SQL_SUCCESS;
            ret = SQLExecute(_hStmt);
            if (!SQL_SUCCEEDED(ret)) {
                // handle error
                return std::nullopt;
            }

            // check if there are any items
            SQLSMALLINT num_items{ 0 };
            ret = SQLNumResultCols(_hStmt, &num_items);
            if (!SQL_SUCCEEDED(ret)) {
                // handle error
                return std::nullopt;
            }

            if (num_items == 0) {
                // the player has neither items in the inventory nor equipped
                return { {} }; // lol?
            }

            // fetch data
            std::vector<PlayerOwnItem> items;
            _item.player_id = _player_id;
            while (SQL_SUCCESS == SQLFetch(_hStmt)) {
                items.push_back(std::move(_item));  // move the item 
            }
            SQLCloseCursor(_hStmt);

            return std::move(items); 
        }
    }
}
