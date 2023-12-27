#include "Database/Statements/GetMonsterGenerators.h"

namespace Eternal
{
    namespace Database
    {
        GetMonsterGenerators::GetMonsterGenerators(SQLHANDLE hCon)
            : _gen{ 0 }
        {
            SQLAllocHandle(SQL_HANDLE_STMT, hCon, &_hStmt);
        }

        SQLRETURN GetMonsterGenerators::bind()
        {
            // prepare the statement
            // get_generators()
            static std::string stmt = "EXEC get_generators";
            TRYODBC(_hStmt, SQL_HANDLE_STMT,
                SQLPrepare(_hStmt, (SQLCHAR*)stmt.c_str(), stmt.size()));

            // bind columns
            // gen.id
            TRYODBC(_hStmt, SQL_HANDLE_STMT,
                SQLBindCol(_hStmt, 1, SQL_C_ULONG,  &_gen.id,          0, nullptr));
            // gen.map_id                                              
            TRYODBC(_hStmt, SQL_HANDLE_STMT,                           
                SQLBindCol(_hStmt, 2, SQL_C_ULONG,  &_gen.map_id,      0, nullptr));
            // gen.bound_x                                             
            TRYODBC(_hStmt, SQL_HANDLE_STMT,                           
                SQLBindCol(_hStmt, 3, SQL_C_USHORT, &_gen.bound_x,     0, nullptr));
            // gen.bound_y                                             
            TRYODBC(_hStmt, SQL_HANDLE_STMT,                           
                SQLBindCol(_hStmt, 4, SQL_C_USHORT, &_gen.bound_y,     0, nullptr));
            // gen.bound_cx                                            
            TRYODBC(_hStmt, SQL_HANDLE_STMT,                           
                SQLBindCol(_hStmt, 5, SQL_C_USHORT, &_gen.bound_cx,    0, nullptr));
            // gen.bound_cy                                            
            TRYODBC(_hStmt, SQL_HANDLE_STMT,                           
                SQLBindCol(_hStmt, 6, SQL_C_USHORT, &_gen.bound_cy,    0, nullptr));
            // gen.max_npc                                             
            TRYODBC(_hStmt, SQL_HANDLE_STMT,                           
                SQLBindCol(_hStmt, 7, SQL_C_USHORT, &_gen.max_npc,     0, nullptr));
            // gen.reset_secs                                          
            TRYODBC(_hStmt, SQL_HANDLE_STMT,                           
                SQLBindCol(_hStmt, 8, SQL_C_USHORT, &_gen.reset_secs,  0, nullptr));
            // gen.max_per_gen
            TRYODBC(_hStmt, SQL_HANDLE_STMT,
                SQLBindCol(_hStmt, 9, SQL_C_USHORT, &_gen.max_per_gen, 0, nullptr));
            // gen.monster_type
            TRYODBC(_hStmt, SQL_HANDLE_STMT,
                SQLBindCol(_hStmt, 10, SQL_C_ULONG, &_gen.monster_type, 0, nullptr));

            return SQL_SUCCESS;

        bailout:
            return SQL_ERROR;
        }
        std::optional<std::vector<MonsterGenerator>> GetMonsterGenerators::execute()
        {
            SQLRETURN res = SQL_SUCCESS;
            res = SQLExecute(_hStmt);
            if (!SQL_SUCCEEDED(res)) {
                return std::nullopt;
            }
            
            res = SQLFetch(_hStmt);
            if (SQL_NO_DATA == res) {
                return std::nullopt;
            }

            std::vector<MonsterGenerator> gens;
            do {
                gens.push_back(_gen);
            } while (SQL_SUCCESS == SQLFetch(_hStmt));

            SQLCloseCursor(_hStmt);
            return std::optional<std::vector<MonsterGenerator>>(std::move(gens));
        }
    }
}
