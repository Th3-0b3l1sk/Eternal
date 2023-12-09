#pragma once
#include "db_helper.h"
#include "./Statements/Register.h"
#include "./Statements/GetAccountInfo.h"
#include "./Statements/GetPlayerOwnItems.h"
#include "./Statements/GetPlayerInfo.h"
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
            AccountInfo(int32_t id, std::string account_id, std::string password, std::string ip,
                AccountType type, bool is_online)
                :_id{ id }, _account_id {account_id}, _password{ password }, _ip{ ip },
                _type{ type }, _is_online{ is_online }
            {

            }

            AccountInfo() = delete;

            int32_t _id;
            std::string _account_id;
            std::string _password;
            std::string _ip;
            AccountType _type;
            bool _is_online;
        };

        class Database
        {
        public:
            Database();
            Database(std::string_view dsn_name, std::string_view user_name, std::string_view password);
            ~Database() = default;

        public:
            void connect(std::string_view n, std::string_view w);
            void load_statements(std::string_view file);

        public:
            void update_player_jump(uint32_t player_id, uint16_t new_x, uint16_t new_y);
            std::optional<AccountInfo> get_account_info(std::string account_id);
            std::optional<PlayerInfo> get_player_info(uint32_t player_id);
            bool register_user(std::string name, std::string password, std::string ip, AccountType type = AccountType::NORMAL);
            std::optional<std::vector<PlayerOwnItem>> get_player_own_items(uint32_t player_id);
            


        private:
            SQLHANDLE _hEnv;
            SQLHANDLE _hCon;
            std::unordered_map<std::string, std::string> _statements;
        };
    }
}
