#pragma once
#include <cstdint>
#include <string>
#include <memory>
#include <vector>
#include "../db_helper.h"

namespace Eternal
{
    namespace Database
    {
        enum class StatementID
        {
            AUTHENTICATE   = 0,
            REGISTER       = 1,
            GET_USER       = 2,
            GET_USER_ITEMS = 3,
            GET_ITEMTYPE   = 4,
            GET_MAP        = 5,

        };

        /* statements are always executed within the context of a database */
        class IStatement
        {
        public:
            IStatement(StatementID id)
                : _id(id), _info(nullptr), _hStatement(SQL_NULL_HANDLE)
            {
            }
            virtual ~IStatement() {
                SQLFreeHandle(SQL_HANDLE_STMT, _hStatement);
            }

        public:
            virtual SQLRETURN  bind() = 0;
            virtual std::vector<std::unique_ptr<uint8_t[]>> fetch() = 0;
            virtual void hook_stmt(std::string& stmt)
            {
                // to override default SQL statements
            }

        public:
            inline StatementID get_id() const { return _id; }
            inline SQLHANDLE& get_handle() { return _hStatement; }

        protected:
            std::unique_ptr<uint8_t[]> _info;
            SQLHANDLE _hStatement;
            StatementID _id;
        };
    }
}
