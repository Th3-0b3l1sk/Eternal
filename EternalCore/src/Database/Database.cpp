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

        std::vector<std::unique_ptr<uint8_t[]>> Database::execute(std::unique_ptr<IStatement>&& statement)
        {
            auto stmt_id  = statement->get_id();
            auto& stmt_h = statement->get_handle();
            auto& stmt_txt = _statements.at(id_to_stmt(stmt_id).data());
                        
            statement->hook_stmt(stmt_txt);
            TRYODBC(stmt_h, SQL_HANDLE_STMT,
                SQLAllocHandle(SQL_HANDLE_STMT, _hCon, &stmt_h));
            TRYODBC(stmt_h, SQL_HANDLE_STMT,
                SQLPrepare(stmt_h, (SQLCHAR*)stmt_txt.c_str(), stmt_txt.size()));
            TRYODBC(stmt_h, SQL_HANDLE_STMT,
                statement->bind());
            TRYODBC(stmt_h, SQL_HANDLE_STMT,
                SQLExecDirectA(stmt_h, (SQLCHAR*)stmt_txt.c_str(), stmt_txt.size()));
             
            return std::move(statement->fetch());

        bailout:
            return {};
        }

        std::string_view Database::id_to_stmt(StatementID id) const
        {
            switch (id)
            {
            case Eternal::Database::StatementID::AUTHENTICATE:
                return "authenticate";
            case Eternal::Database::StatementID::REGISTER:
                return "register";
            case Eternal::Database::StatementID::GET_USER:
                return "get_user";
            case Eternal::Database::StatementID::GET_USER_ITEMS:
                return "get_user_items";
            case Eternal::Database::StatementID::GET_ITEMTYPE:
                return "get_itemtype";
            case Eternal::Database::StatementID::GET_MAP:
                return "get_map";
            default:
                throw std::exception{ "Invalid statement id" };
            }
        }
    }
}
