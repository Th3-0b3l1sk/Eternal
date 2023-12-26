#include "Database/Statements/GetMonsterType.h"

namespace Eternal
{
    namespace Database
    {
        GetMonsterType::GetMonsterType(SQLHANDLE hCon)
        {
            SQLAllocHandle(SQL_HANDLE_STMT, hCon, &_hStmt);
            _m_type.name.resize(MAX_NAME_LEN);
        }

        SQLRETURN GetMonsterType::bind()
        {
            // prepare the statement
            // get_monster_type
            static std::string stmt = "EXEC get_monster_type";
            TRYODBC(_hStmt, SQL_HANDLE_STMT,
                SQLPrepare(_hStmt, (SQLCHAR*)stmt.c_str(), stmt.size()));


            // Binding columns
            TRYODBC(_hStmt, SQL_HANDLE_STMT,
                SQLBindCol(_hStmt, 1, SQL_C_ULONG,    &_m_type.id,                                       0, nullptr));

            TRYODBC(_hStmt, SQL_HANDLE_STMT,
                SQLBindCol(_hStmt, 2, SQL_C_CHAR,     (SQLPOINTER)_m_type.name.data(), _m_type.name.size(), nullptr));

            TRYODBC(_hStmt, SQL_HANDLE_STMT,
                SQLBindCol(_hStmt, 3, SQL_C_TINYINT,  &_m_type.type,                                     0, nullptr));
                                                                                                         
            TRYODBC(_hStmt, SQL_HANDLE_STMT,                                                             
                SQLBindCol(_hStmt, 4, SQL_C_TINYINT,  &_m_type.ai_type,                                  0, nullptr));
                                                                                                         
            TRYODBC(_hStmt, SQL_HANDLE_STMT,                                                             
                SQLBindCol(_hStmt, 5, SQL_C_ULONG,    &_m_type.look,                                     0, nullptr));
                                                                                                         
            TRYODBC(_hStmt, SQL_HANDLE_STMT,                                                             
                SQLBindCol(_hStmt, 6, SQL_C_ULONG,    &_m_type.level,                                    0, nullptr));
                                                                                                         
            TRYODBC(_hStmt, SQL_HANDLE_STMT,                                                             
                SQLBindCol(_hStmt, 7, SQL_C_ULONG,    &_m_type.life,                                     0, nullptr));
                                                                                                         
            TRYODBC(_hStmt, SQL_HANDLE_STMT,                                                             
                SQLBindCol(_hStmt, 8, SQL_C_ULONG,    &_m_type.escape_life,                              0, nullptr));
                                                                                                         
            TRYODBC(_hStmt, SQL_HANDLE_STMT,                                                             
                SQLBindCol(_hStmt, 9, SQL_C_TINYINT,  &_m_type.attack_user,                              0, nullptr));

            TRYODBC(_hStmt, SQL_HANDLE_STMT,
                SQLBindCol(_hStmt, 10, SQL_C_ULONG,   &_m_type.attack_min,                               0, nullptr));
                                                                                                         
            TRYODBC(_hStmt, SQL_HANDLE_STMT,                                                             
                SQLBindCol(_hStmt, 11, SQL_C_ULONG,   &_m_type.attack_max,                               0, nullptr));
                                                                                                         
            TRYODBC(_hStmt, SQL_HANDLE_STMT,                                                             
                SQLBindCol(_hStmt, 12, SQL_C_ULONG,   &_m_type.defense,                                  0, nullptr));
                                                                                                         
            TRYODBC(_hStmt, SQL_HANDLE_STMT,                                                             
                SQLBindCol(_hStmt, 13, SQL_C_SHORT,   &_m_type.dexterity,                                0, nullptr));

            TRYODBC(_hStmt, SQL_HANDLE_STMT,
                SQLBindCol(_hStmt, 14, SQL_C_SHORT,   &_m_type.dodge,                                    0, nullptr));
                                                                                                         
            TRYODBC(_hStmt, SQL_HANDLE_STMT,                                                             
                SQLBindCol(_hStmt, 15, SQL_C_ULONG,   &_m_type.magic_type,                               0, nullptr));
                                                                                                         
            TRYODBC(_hStmt, SQL_HANDLE_STMT,                                                             
                SQLBindCol(_hStmt, 16, SQL_C_ULONG,   &_m_type.magic_def,                                0, nullptr));

            TRYODBC(_hStmt, SQL_HANDLE_STMT,
                SQLBindCol(_hStmt, 17, SQL_C_ULONG,   &_m_type.magic_hit_rate,                           0, nullptr));
                                                                                                         
            TRYODBC(_hStmt, SQL_HANDLE_STMT,                                                             
                SQLBindCol(_hStmt, 18, SQL_C_TINYINT, &_m_type.view_range,                               0, nullptr));
                                                                                                         
            TRYODBC(_hStmt, SQL_HANDLE_STMT,                                                             
                SQLBindCol(_hStmt, 19, SQL_C_TINYINT, &_m_type.attack_range,                             0, nullptr));
                                                                                                         
            TRYODBC(_hStmt, SQL_HANDLE_STMT,                                                             
                SQLBindCol(_hStmt, 20, SQL_C_SHORT,   &_m_type.attack_speed,                             0, nullptr));

            TRYODBC(_hStmt, SQL_HANDLE_STMT,
                SQLBindCol(_hStmt, 21, SQL_C_SHORT,   &_m_type.move_speed,                               0, nullptr));
                                                                                                         
            TRYODBC(_hStmt, SQL_HANDLE_STMT,                                                             
                SQLBindCol(_hStmt, 22, SQL_C_SHORT,   &_m_type.run_speed,                                0, nullptr));
                                                                                                         
            TRYODBC(_hStmt, SQL_HANDLE_STMT,                                                             
                SQLBindCol(_hStmt, 23, SQL_C_SHORT,   &_m_type.drop_armet,                               0, nullptr));

            TRYODBC(_hStmt, SQL_HANDLE_STMT,
                SQLBindCol(_hStmt, 24, SQL_C_SHORT,   &_m_type.drop_necklace,                            0, nullptr));
                                                                                                         
            TRYODBC(_hStmt, SQL_HANDLE_STMT,                                                             
                SQLBindCol(_hStmt, 25, SQL_C_SHORT,   &_m_type.drop_armor,                               0, nullptr));
                                                                                                         
            TRYODBC(_hStmt, SQL_HANDLE_STMT,                                                             
                SQLBindCol(_hStmt, 26, SQL_C_SHORT,   &_m_type.drop_ring,                                0, nullptr));
                                                                                                         
            TRYODBC(_hStmt, SQL_HANDLE_STMT,                                                             
                SQLBindCol(_hStmt, 27, SQL_C_SHORT,   &_m_type.drop_weapon,                              0, nullptr));
                                                                                                         
            TRYODBC(_hStmt, SQL_HANDLE_STMT,                                                             
                SQLBindCol(_hStmt, 28, SQL_C_SHORT,   &_m_type.drop_shield,                              0, nullptr));
                                                                                                         
            TRYODBC(_hStmt, SQL_HANDLE_STMT,                                                             
                SQLBindCol(_hStmt, 29, SQL_C_SHORT,   &_m_type.drop_shoes,                               0, nullptr));
                                                                                                         
            TRYODBC(_hStmt, SQL_HANDLE_STMT,                                                             
                SQLBindCol(_hStmt, 30, SQL_C_ULONG,   &_m_type.drop_money,                               0, nullptr));

            TRYODBC(_hStmt, SQL_HANDLE_STMT,
                SQLBindCol(_hStmt, 31, SQL_C_ULONG,   &_m_type.drop_hp,                                  0, nullptr));
                                                                                                         
            TRYODBC(_hStmt, SQL_HANDLE_STMT,                                                             
                SQLBindCol(_hStmt, 32, SQL_C_ULONG,   &_m_type.drop_mp,                                  0, nullptr));

            TRYODBC(_hStmt, SQL_HANDLE_STMT,
                SQLBindCol(_hStmt, 33, SQL_C_ULONG,   &_m_type.extra_battle_level,                       0, nullptr));
                                                                                                         
            TRYODBC(_hStmt, SQL_HANDLE_STMT,                                                             
                SQLBindCol(_hStmt, 34, SQL_C_ULONG,   &_m_type.extra_exp,                                0, nullptr));
                                                                                                         
            TRYODBC(_hStmt, SQL_HANDLE_STMT,                                                             
                SQLBindCol(_hStmt, 35, SQL_C_SHORT,   &_m_type.extra_damage,                             0, nullptr));


            // No parameters to bind, fetching all
            return SQL_SUCCESS;

        bailout:
            return SQL_ERROR;
        }

        std::optional<std::vector<MonsterType>> GetMonsterType::execute()
        {
            SQLRETURN res = SQL_SUCCESS;
            res = SQLExecute(_hStmt);
            if (!SQL_SUCCEEDED(res)) {
                return std::nullopt;
            }

            res = SQLFetch(_hStmt);
            if (SQL_NO_DATA == res) {
                std::nullopt;
            }

            std::vector<MonsterType> types;
            do
            {
                types.push_back(_m_type);
            } while (SQL_SUCCESS == SQLFetch(_hStmt));

            SQLCloseCursor(_hStmt);
            return std::optional<std::vector<MonsterType>>(std::move(types));
        }
    }
}
