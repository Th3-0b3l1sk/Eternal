#include "Util\LineReader.h"
#include <iostream>
#include <sstream>
#include <filesystem>

namespace Eternal
{
    namespace Util
    {
        static bool is_all_backspaces(const std::string& str) {
            for (char c : str) {
                if (c != '\b') {
                    return false;
                }
            }
            return true;
        }

        LineReader::LineReader(std::string_view file, char sep)
        {
            if (!std::filesystem::exists(file.data()))
                throw std::exception{ "File not found" };

            _file = std::ifstream(file.data());
            if (!_file.is_open())
                throw std::exception{ "Failed to open the file" };
        }

        LineReader::~LineReader()
        {
            _file.close();
        }

        // trailing and leading spaces are not removed. at least for now.
        std::unordered_map<std::string, std::string>&& LineReader::get_lines()
        {
            std::stringstream ss;
            ss << _file.rdbuf();
            std::string line;
            size_t pos = std::string::npos;
            while (std::getline(ss, line) && !is_all_backspaces(line)) {
                pos = line.find('=');
                if (pos == std::string::npos)
                    throw std::exception{ "Ill-formed file" };
                // for all purposes, a repeated key is an ill-formed file
                if (!_values.insert({ line.substr(0, pos), pos > line.length() ? "" : line.substr(pos + 1) }).second)
                    throw std::exception{ "Repeated key" }; 
            }

            return std::move(_values);
        }

    }

}

