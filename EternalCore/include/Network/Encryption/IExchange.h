#pragma once
#include <string>

namespace Eternal
{
    namespace Encryption
    {
        class IExchange
        {
        public:
            virtual ~IExchange() = default;
            virtual void generate_key() = 0;
            virtual std::string derive_shared_secret(std::string_view) = 0;;
            virtual std::string get_private_key() = 0;
            virtual std::string get_public_key() = 0;
        };
    }
}


