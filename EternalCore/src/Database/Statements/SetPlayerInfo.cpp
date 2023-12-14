#include "Database/Statements/SetPlayerInfo.h"

namespace Eternal
{
    namespace Database
    {
        SetPlayerInfo::SetPlayerInfo(SQLHANDLE hCon)
            : _hStmt{ NULL }, _info{}
        {
            SQLAllocHandle(SQL_HANDLE_STMT, hCon, &_hStmt);
            _info.name.resize(MAX_NAME_LEN);
            _info.mate.resize(MAX_NAME_LEN);
        }

        SQLRETURN SetPlayerInfo::bind()
        {
            // prepare the statment
            // set_player_info(account_id, identity, name, mate, lookface, hair, money, money_save, cps, level,
            //            mana, profession, pk, virtue, nobility, rebirth, syn_id, record_map, record_x, record_y,
            //            last_login);

            static std::string stmt = "EXEC set_player_info @account_id=?, @identity=?, @name=?, @mate=?,"
                "@lookface=?, @hair=?, @money=?, @money_save=?, @cps=?, @level=?, @exp=?, @force=?, @dex=?,"
                "@health=?, @soul=?, @add_points=?, @life=?, @mana=?, @profession=?, @pk=?, @virtue=?,"
                "@nobility=?, @rebirth=?, @syn_id=?, @record_map=?, @record_x=?, @record_y=?, @last_login=?";

            if (SQL_SUCCESS != SQLPrepare(_hStmt, (SQLCHAR*)stmt.c_str(), stmt.size())) {
                // todo: handle error;
                return SQL_ERROR;
            }

            // account_id
            TRYODBC(_hStmt, SQL_HANDLE_STMT,
                SQLBindParameter(_hStmt,
                    1,
                    SQL_PARAM_INPUT, C_DWORD, SQL_INTEGER,
                    NULL, NULL,
                    (SQLPOINTER)&_info.id,
                    NULL, NULL));
            // identity
            TRYODBC(_hStmt, SQL_HANDLE_STMT,
                SQLBindParameter(_hStmt,
                    2,
                    SQL_PARAM_INPUT, C_DWORD, SQL_INTEGER,
                    NULL, NULL,
                    (SQLPOINTER)&_info.identity,
                    NULL, NULL));

            // name
            TRYODBC(_hStmt, SQL_HANDLE_STMT,
                SQLBindParameter(_hStmt,
                    3,
                    SQL_PARAM_INPUT, C_STR, SQL_VARCHAR,
                    NULL, NULL,
                    (SQLPOINTER)_info.name.c_str(),
                    _info.name.size(), NULL));

            // mate
            TRYODBC(_hStmt, SQL_HANDLE_STMT,
                SQLBindParameter(_hStmt,
                    4,
                    SQL_PARAM_INPUT, C_STR, SQL_VARCHAR,
                    NULL, NULL,
                    (SQLPOINTER)_info.mate.c_str(),
                    _info.mate.size(), NULL));

            // lookface
            TRYODBC(_hStmt, SQL_HANDLE_STMT,
                SQLBindParameter(_hStmt,
                    5,
                    SQL_PARAM_INPUT, C_DWORD, SQL_INTEGER,
                    NULL, NULL,
                    (SQLPOINTER)&_info.lookface,
                    NULL, NULL));

            // hair
            TRYODBC(_hStmt, SQL_HANDLE_STMT,
                SQLBindParameter(_hStmt,
                    6,
                    SQL_PARAM_INPUT, C_WORD, SQL_SMALLINT,
                    NULL, NULL,
                    (SQLPOINTER)&_info.hair,
                    NULL, NULL));

            // money
            TRYODBC(_hStmt, SQL_HANDLE_STMT,
                SQLBindParameter(_hStmt,
                    7,
                    SQL_PARAM_INPUT, C_DWORD, SQL_INTEGER,
                    NULL, NULL,
                    (SQLPOINTER)&_info.money,
                    NULL, NULL));

            // money_saved
            TRYODBC(_hStmt, SQL_HANDLE_STMT,
                SQLBindParameter(_hStmt,
                    8,
                    SQL_PARAM_INPUT, C_DWORD, SQL_INTEGER,
                    NULL, NULL,
                    (SQLPOINTER)&_info.money_saved,
                    NULL, NULL));

            // cps
            TRYODBC(_hStmt, SQL_HANDLE_STMT,
                SQLBindParameter(_hStmt,
                    9,
                    SQL_PARAM_INPUT, C_DWORD, SQL_INTEGER,
                    NULL, NULL,
                    (SQLPOINTER)&_info.cps,
                    NULL, NULL));

            // level
            TRYODBC(_hStmt, SQL_HANDLE_STMT,
                SQLBindParameter(_hStmt,
                    10,
                    SQL_PARAM_INPUT, C_BYTE, SQL_TINYINT,
                    NULL, NULL,
                    (SQLPOINTER)&_info.level,
                    NULL, NULL));

            // exp
            TRYODBC(_hStmt, SQL_HANDLE_STMT,
                SQLBindParameter(_hStmt,
                    11,
                    SQL_PARAM_INPUT, C_QWORD, SQL_BIGINT,
                    NULL, NULL,
                    (SQLPOINTER)&_info.exp,
                    NULL, NULL));

            // force
            TRYODBC(_hStmt, SQL_HANDLE_STMT,
                SQLBindParameter(_hStmt,
                    12,
                    SQL_PARAM_INPUT, C_WORD, SQL_SMALLINT,
                    NULL, NULL,
                    (SQLPOINTER)&_info.force,
                    NULL, NULL));

            // dexterity
            TRYODBC(_hStmt, SQL_HANDLE_STMT,
                SQLBindParameter(_hStmt,
                    13,
                    SQL_PARAM_INPUT, C_WORD, SQL_SMALLINT,
                    NULL, NULL,
                    (SQLPOINTER)&_info.dexterity,
                    NULL, NULL));
            // health
            TRYODBC(_hStmt, SQL_HANDLE_STMT,
                SQLBindParameter(_hStmt,
                    14,
                    SQL_PARAM_INPUT, C_WORD, SQL_SMALLINT,
                    NULL, NULL,
                    (SQLPOINTER)&_info.health,
                    NULL, NULL));
            // soul
            TRYODBC(_hStmt, SQL_HANDLE_STMT,
                SQLBindParameter(_hStmt,
                    15,
                    SQL_PARAM_INPUT, C_WORD, SQL_SMALLINT,
                    NULL, NULL,
                    (SQLPOINTER)&_info.soul,
                    NULL, NULL));
            // add_points
            TRYODBC(_hStmt, SQL_HANDLE_STMT,
                SQLBindParameter(_hStmt,
                    16,
                    SQL_PARAM_INPUT, C_WORD, SQL_SMALLINT,
                    NULL, NULL,
                    (SQLPOINTER)&_info.add_points,
                    NULL, NULL));
            // life
            TRYODBC(_hStmt, SQL_HANDLE_STMT,
                SQLBindParameter(_hStmt,
                    17,
                    SQL_PARAM_INPUT, C_WORD, SQL_SMALLINT,
                    NULL, NULL,
                    (SQLPOINTER)&_info.life,
                    NULL, NULL));
            // mana
            TRYODBC(_hStmt, SQL_HANDLE_STMT,
                SQLBindParameter(_hStmt,
                    18,
                    SQL_PARAM_INPUT, C_WORD, SQL_SMALLINT,
                    NULL, NULL,
                    (SQLPOINTER)&_info.mana,
                    NULL, NULL));
            // profession
            TRYODBC(_hStmt, SQL_HANDLE_STMT,
                SQLBindParameter(_hStmt,
                    19,
                    SQL_PARAM_INPUT, C_BYTE, SQL_TINYINT,
                    NULL, NULL,
                    (SQLPOINTER)&_info.profession,
                    NULL, NULL));
            // pk_points
            TRYODBC(_hStmt, SQL_HANDLE_STMT,
                SQLBindParameter(_hStmt,
                    20,
                    SQL_PARAM_INPUT, C_WORD, SQL_SMALLINT,
                    NULL, NULL,
                    (SQLPOINTER)&_info.pk_points,
                    NULL, NULL));
            // virtue
            TRYODBC(_hStmt, SQL_HANDLE_STMT,
                SQLBindParameter(_hStmt,
                    21,
                    SQL_PARAM_INPUT, C_DWORD, SQL_INTEGER,
                    NULL, NULL,
                    (SQLPOINTER)&_info.virtue,
                    NULL, NULL));
            // nobility
            TRYODBC(_hStmt, SQL_HANDLE_STMT,
                SQLBindParameter(_hStmt,
                    22,
                    SQL_PARAM_INPUT, C_BYTE, SQL_TINYINT,
                    NULL, NULL,
                    (SQLPOINTER)&_info.nobility,
                    NULL, NULL));
            // rebirth
            TRYODBC(_hStmt, SQL_HANDLE_STMT,
                SQLBindParameter(_hStmt,
                    23,
                    SQL_PARAM_INPUT, C_BYTE, SQL_TINYINT,
                    NULL, NULL,
                    (SQLPOINTER)&_info.rebirth,
                    NULL, NULL));
            // syndicate_id
            TRYODBC(_hStmt, SQL_HANDLE_STMT,
                SQLBindParameter(_hStmt,
                    24,
                    SQL_PARAM_INPUT, C_WORD, SQL_SMALLINT,
                    NULL, NULL,
                    (SQLPOINTER)&_info.syndicate_id,
                    NULL, NULL));
            // record_map
            TRYODBC(_hStmt, SQL_HANDLE_STMT,
                SQLBindParameter(_hStmt,
                    25,
                    SQL_PARAM_INPUT, C_DWORD, SQL_INTEGER,
                    NULL, NULL,
                    (SQLPOINTER)&_info.record_map,
                    NULL, NULL));
            // record_x
            TRYODBC(_hStmt, SQL_HANDLE_STMT,
                SQLBindParameter(_hStmt,
                    26,
                    SQL_PARAM_INPUT, C_WORD, SQL_SMALLINT,
                    NULL, NULL,
                    (SQLPOINTER)&_info.record_x,
                    NULL, NULL));
            // record_y
            TRYODBC(_hStmt, SQL_HANDLE_STMT,
                SQLBindParameter(_hStmt,
                    27,
                    SQL_PARAM_INPUT, C_WORD, SQL_SMALLINT,
                    NULL, NULL,
                    (SQLPOINTER)&_info.record_y,
                    NULL, NULL));
            // last_login
            TRYODBC(_hStmt, SQL_HANDLE_STMT,
                SQLBindParameter(_hStmt,
                    28,
                    SQL_PARAM_INPUT, C_QWORD, SQL_BIGINT,
                    NULL, NULL,
                    (SQLPOINTER)&_info.last_login,
                    NULL, NULL));

            return SQL_SUCCESS;
        bailout:
            return SQL_ERROR;
        }
        bool SetPlayerInfo::execute()
        {
            SQLRETURN ret = SQL_SUCCESS;

            ret = SQLExecute(_hStmt);
            if (!SQL_SUCCEEDED(ret)) {
                std::cout << get_error(_hStmt, SQL_HANDLE_STMT, ret) << '\n';
                return false;
            }

            return true;
        }
        SetPlayerInfo& SetPlayerInfo::set_id(uint32_t id)
        {
            _info.id = id;
            _info.identity = id + IDENTITY_START;

            return *this;
        }
        SetPlayerInfo& SetPlayerInfo::set_name(const std::string& name)
        {
            // TODO: perform name checks here, throw if bad
            if (name.size() >= MAX_NAME_LEN)
                throw std::exception{};

            _info.name = name;
            return *this;
        }
        SetPlayerInfo& SetPlayerInfo::set_mate(const std::string& mate)
        {
            // TODO: insert return statement here
            if (mate.size() >= MAX_NAME_LEN)
                throw std::exception{};
            _info.mate = mate;
            return *this;
        }
        SetPlayerInfo& SetPlayerInfo::set_lookface(uint32_t lookface)
        {
            // TODO: checks here
            _info.lookface = lookface;
            return *this;
        }
        SetPlayerInfo& SetPlayerInfo::set_hair(uint16_t hair)
        {
            // TODO: insert return statement here
            _info.hair = hair;
            return *this;
        }
        SetPlayerInfo& SetPlayerInfo::set_money(uint32_t money)
        {
            // TODO: insert return statement here
            _info.money = money;
            return *this;
        }
        SetPlayerInfo& SetPlayerInfo::set_money_saved(uint32_t money_saved)
        {
            // TODO: insert return statement here
            _info.money_saved = money_saved;
            return *this;
        }
        SetPlayerInfo& SetPlayerInfo::set_cps(uint32_t cps)
        {
            // TODO: insert return statement here
            _info.cps = cps;
            return *this;
        }
        SetPlayerInfo& SetPlayerInfo::set_level(uint8_t level)
        {
            // TODO: insert return statement here
            _info.level = level;
            return *this;
        }
        SetPlayerInfo& SetPlayerInfo::set_exp(uint64_t exp)
        {
            // TODO: insert return statement here
            _info.exp = exp;
            return *this;
        }
        SetPlayerInfo& SetPlayerInfo::set_force(uint16_t force)
        {
            // TODO: insert return statement here
            _info.force = force;
            return *this;
        }
        SetPlayerInfo& SetPlayerInfo::set_dexterity(uint16_t dex)
        {
            // TODO: insert return statement here
            _info.dexterity = dex;
            return *this;
        }
        SetPlayerInfo& SetPlayerInfo::set_health(uint16_t health)
        {
            // TODO: insert return statement here
            _info.health = health;
            return *this;
        }
        SetPlayerInfo& SetPlayerInfo::set_soul(uint16_t soul)
        {
            // TODO: insert return statement here
            _info.soul = soul;
            return *this;
        }
        SetPlayerInfo& SetPlayerInfo::set_add_points(uint16_t add_points)
        {
            // TODO: insert return statement here
            _info.add_points = add_points;
            return *this;
        }
        SetPlayerInfo& SetPlayerInfo::set_life(uint16_t life)
        {
            // TODO: insert return statement here
            _info.life = life;
            return *this;
        }
        SetPlayerInfo& SetPlayerInfo::set_mana(uint16_t mana)
        {
            // TODO: insert return statement here
            _info.mana = mana;
            return *this;
        }
        SetPlayerInfo& SetPlayerInfo::set_profession(uint8_t prof)
        {
            // TODO: insert return statement here
            _info.profession = prof;
            return *this;
        }
        SetPlayerInfo& SetPlayerInfo::set_pk_points(uint16_t pk_points)
        {
            // TODO: insert return statement here
            _info.pk_points = pk_points;
            return *this;
        }
        SetPlayerInfo& SetPlayerInfo::set_virtue(uint32_t virtue)
        {
            // TODO: insert return statement here
            _info.virtue = virtue;
            return *this;
        }
        SetPlayerInfo& SetPlayerInfo::set_nobility(uint8_t nobility)
        {
            // TODO: insert return statement here
            _info.nobility = nobility;
            return *this;
        }
        SetPlayerInfo& SetPlayerInfo::set_rebirth(uint8_t rebirth)
        {
            // TODO: insert return statement here
            _info.rebirth = rebirth;
            return *this;
        }
        SetPlayerInfo& SetPlayerInfo::set_syndicate_id(uint16_t syn_id)
        {
            // TODO: insert return statement here
            _info.syndicate_id = syn_id;
            return *this;
        }
        SetPlayerInfo& SetPlayerInfo::set_record_map(uint32_t record_map)
        {
            // TODO: insert return statement here
            _info.record_map = record_map;
            return *this;
        }
        SetPlayerInfo& SetPlayerInfo::set_record_x(uint16_t record_x)
        {
            // TODO: insert return statement here
            _info.record_x = record_x;
            return *this;
        }
        SetPlayerInfo& SetPlayerInfo::set_record_y(uint16_t record_y)
        {
            // TODO: insert return statement here
            _info.record_y = record_y;
            return *this;
        }
        SetPlayerInfo& SetPlayerInfo::set_last_login(uint64_t last_login)
        {
            // TODO: insert return statement here
            _info.last_login = last_login;
            return *this;
        }
    }
}
