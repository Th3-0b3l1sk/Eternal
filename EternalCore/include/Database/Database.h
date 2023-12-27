#pragma once
#include "db_helper.h"
#include "./Statements/Register.h"
#include "./Statements/GetAccountInfo.h"
#include "./Statements/GetPlayerOwnItems.h"
#include "./Statements/GetPlayerInfo.h"
#include "./Statements/GetItemsInfo.h"
#include "./Statements/GetNpcsInfo.h"
#include "./Statements/GetMapsInfo.h"
#include "./Statements/GetMonsterType.h"
#include "./Statements/GetMonsterGenerators.h"
#include <string>
#include <unordered_map>
#include <optional>

namespace Eternal
{
    namespace Database
    {
        enum class AccountType : uint8_t
        {
            GM = 0,
            PM = 1,
            NORMAL = 2
        };

        // GetAccountInfo.h
        struct AccountInfo
        {
            AccountInfo(uint32_t id, std::string account_id, std::string password, std::string ip,
                AccountType type, bool is_online)
                :_id{ id }, _account_id {account_id}, _password{ password }, _ip{ ip },
                _type{ type }, _is_online{ is_online }
            {

            }

            uint32_t get_player_id() const { return _id; }

            AccountInfo() = delete;

            uint32_t _id;
            std::string _account_id;
            std::string _password;
            std::string _ip;
            AccountType _type;
            bool _is_online;
        };

        class Database
        {
            static PlayerInfo _s_default_info;
        public:
            Database();
            Database(std::string_view dsn_name, std::string_view user_name, std::string_view password);
            ~Database() = default;

        public:
            void connect(std::string_view n, std::string_view w);

        public:
            void update_player_jump(uint32_t player_id, uint16_t new_x, uint16_t new_y);
            std::optional<AccountInfo> get_account_info(std::string account_id);
            std::optional<PlayerInfo> get_player_info(uint32_t player_id);
            std::uint32_t get_player_identity(std::string player_name);
            std::optional<std::vector<PlayerOwnItem>> get_player_own_items(uint32_t player_id);
            std::optional<std::vector<ItemInfo>> get_game_items();
            std::optional<std::vector<NpcInfo>> get_game_npcs();
            std::optional<std::vector<MapInfo>> get_game_maps();
            std::optional<std::vector<MonsterType>> get_game_monsters();
            std::optional<std::vector<MonsterGenerator>> get_generators();
            bool register_user(std::string name, std::string password, std::string ip, AccountType type = AccountType::NORMAL);
            bool set_player_info(uint32_t id, const PlayerInfo& info);
            void set_def_player_info(PlayerInfo& info);

        private:
            void load_def_player_info();

        private:
            SQLHANDLE _hEnv;
            SQLHANDLE _hCon;
        };
    }
}
