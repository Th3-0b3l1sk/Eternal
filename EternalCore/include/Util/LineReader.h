#pragma once
#include <unordered_map>
#include <string_view>
#include <fstream>

namespace Eternal
{
    namespace Util
    {
        class LineReader
        {
        public:
            LineReader(std::string_view file, char sep = '=');
            ~LineReader();

        public:
            std::unordered_map<std::string, std::string>&& get_lines();

        private:
            std::unordered_map<std::string, std::string> _values;
            std::ifstream _file;
        };
    }
}
