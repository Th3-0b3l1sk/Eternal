#pragma once
#include <string>

namespace Eternal
{
    namespace Util
    {
        class StrUtils
        {
        public:
            static void trim_trailing_space(std::string& str)
            {
                auto first_space = std::find_if(str.begin(), str.end(), [](char c) { return isspace(c); });
                if (first_space == str.end())
                    return;
                auto new_size = first_space - str.begin();
                str.resize(new_size);
            }

            static void trim_leading_spaces(std::string& str)
            {
                auto first_non_space = std::find_if(str.begin(), str.end(), [](char c) { return !isspace(c); });
                if (first_non_space == str.end())
                    return;
                str = { first_non_space, str.end() };
            }

            static void trim_spaces(std::string& str)
            {
                trim_leading_spaces(str);
                trim_trailing_space(str);
            }

            static bool is_empty(const std::string& str) {
                return std::all_of(str.begin(), str.end(), [](char c) { return isspace(c); });
            }
        };
    }
}
