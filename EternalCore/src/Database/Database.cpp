#include "Database/Database.h"
#include "Database/Statements/GetPlayerIdByName.h"
#include "Database/Statements/SetPlayerInfo.h"
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

        std::uint32_t Database::get_player_identity(std::string player_name)
        {
            GetPlayerIdByName id(_hCon);
            if (SQL_SUCCESS != id.bind()) {
                return -1;
            }

            return id.set_player_name(player_name).execute();
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
        bool Database::set_player_info(uint32_t id, const PlayerInfo& info)
        {
            SetPlayerInfo set_info(_hCon);
            if (!SQL_SUCCEEDED(set_info.bind())) {
                return false;
            }

            // TODO: what the hell
            set_info.set_id(id).set_name(info.name).set_mate(info.mate).set_lookface(info.lookface)
                .set_hair(info.hair).set_money(info.money).set_money_saved(info.money_saved).set_cps(info.cps)
                .set_level(info.level).set_exp(info.exp).set_force(info.force).set_dexterity(info.dexterity)
                .set_health(info.health).set_soul(info.soul).set_add_points(info.add_points).set_life(info.life)
                .set_mana(info.mana).set_profession(info.profession).set_pk_points(info.pk_points).set_virtue(info.virtue)
                .set_nobility(info.nobility).set_rebirth(info.rebirth).set_syndicate_id(info.syndicate_id).set_record_map(info.record_map)
                .set_record_x(info.record_x).set_record_y(info.record_y).set_last_login(info.last_login);

            return set_info.execute();
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
