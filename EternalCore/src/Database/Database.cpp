#include "Database/Database.h"
#include "Util/LineReader.h"
#include <string>
#include <iostream>

namespace Eternal
{
    namespace Database
    {
        Database::Database()
            : _hEnv(SQL_NULL_HENV), _hCon(SQL_NULL_HDBC)
        {
            TRYODBC(_hEnv, SQL_HANDLE_ENV,
                SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &_hEnv));
            TRYODBC(_hEnv, SQL_HANDLE_ENV,
                SQLSetEnvAttr(_hEnv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0));
            TRYODBC(_hCon, SQL_HANDLE_DBC,
                SQLAllocHandle(SQL_HANDLE_DBC, _hEnv, &_hCon));

            return;

            // TRYODBC goes to bailout on failure
        bailout:
            DO_IF(_hEnv != SQL_NULL_HANDLE, SQLFreeHandle, SQL_HANDLE_ENV, _hEnv);
            DO_IF(_hCon != SQL_NULL_HANDLE, SQLFreeHandle, SQL_HANDLE_DBC, _hCon);

            throw std::exception{ "panic" };
        }

        Database::Database(std::string_view dsn_name, std::string_view username, std::string_view password)
            : Database()
        {
            if (_hEnv == SQL_NULL_HANDLE || _hCon == SQL_NULL_HANDLE)
                return;

            TRYODBC(_hCon, SQL_HANDLE_DBC,
                SQLConnectA(_hCon,
                    (SQLCHAR*)dsn_name.data(), (SQLSMALLINT)dsn_name.size(),
                    (SQLCHAR*)username.data(), (SQLSMALLINT)username.size(),
                    (SQLCHAR*)password.data(), (SQLSMALLINT)password.size()));

            std::cout << "[successfully connected to the database!]\n";
            return;

        bailout:
            DO_IF(_hCon != SQL_NULL_HANDLE, SQLFreeHandle, SQL_HANDLE_DBC, _hCon);
            throw std::exception{ "panic" };
        }

        // requires the file to be in the key=value format
        void Database::load_statements(std::string_view file)
        {
            Util::LineReader reader(file);
            _statements = reader.get_lines();          
        }

        void Database::update_player_jump(uint32_t player_id, uint16_t new_x, uint16_t new_y)
        {

        }

        std::optional<AccountInfo> Database::get_account_info(std::string account_id)
        {
            GetAccountInfo info(_hCon);
            if (SQL_SUCCESS != info.bind()) {
                // TODO: failed to bind
                return std::nullopt;
            }
            
            info.set_account_id(account_id);
            auto result = info.execute();
            if (!SQL_SUCCEEDED(result)) {
                // TODO: failed to execute
                return std::nullopt;
            }
   
            return AccountInfo{ info.get_id(), info.let_name(), info.let_password(), 
                info.let_ip(), (AccountType)info.account_type(), info.is_online() };
        }

        std::optional<PlayerInfo> Database::get_player_info(uint32_t player_id)
        {
            GetPlayerInfo info(_hCon);
            if (SQL_SUCCESS != info.bind()) {
                // TODO: failed to bind
                return std::nullopt;
            }

            return info.set_player_id(player_id).execute();
        }

        void authenticate()
        {

        }

        bool Database::register_user(std::string name, std::string password, std::string ip, AccountType type)
        {
            Register register_user(_hCon);
            if (SQL_SUCCESS != register_user.bind()) {
                // TODO: failed to bind params
                return false;
            }

            if (SQL_SUCCESS == register_user
                .set_name(name)
                .set_password(password)
                .set_ip(ip)
                .set_type((uint8_t)type)
                .execute()) 
            {
                return true;
            }

            return false;
        }
        std::optional<std::vector<PlayerOwnItem>> Database::get_player_own_items(uint32_t player_id)
        {
            GetPlayerOwnItems player_items(_hCon);
            if (SQL_SUCCESS != player_items.bind()) {
                // handle errors
                return std::nullopt;
            }
            
            return player_items.set_player_id(player_id).execute();
        }
        std::optional<std::vector<ItemInfo>> Database::get_game_items()
        {
            GetItemsInfo items_info(_hCon);
            if (!SQL_SUCCEEDED(items_info.bind())) {
                // handle error
                return std::nullopt;
            }

            return items_info.execute();

        }
        std::optional<std::vector<NpcInfo>> Database::get_game_npcs()
        {
            GetNpcsInfo npcs_info(_hCon);
            if (!SQL_SUCCEEDED(npcs_info.bind())) {
                // handler error
                return std::nullopt;
            }
            
            return npcs_info.execute();
        }
        std::optional<std::vector<MapInfo>> Database::get_game_maps()
        {
            GetMapsInfo maps_info(_hCon);
            if (!SQL_SUCCEEDED(maps_info.bind())) {
                // handler error
                return std::nullopt;
            }

            return maps_info.execute();
        }
    }
}
