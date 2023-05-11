#include "Util/IniFile.h"
#include "Util/StrUtils.h"
#include <fstream>
#include <sstream>

namespace Eternal
{
    namespace Util
    {
        const std::string IniFile::GLOBAL = "GLOBAL";

#define SECTION_BEGIN   '['
#define SECTION_END     ']'
#define COMMENT_BEGIN   ';'
#define SEPARATOR       '='

        static bool is_empty_or_comment(const std::string& str)
        {
            return str[0] == COMMENT_BEGIN || StrUtils::is_empty(str);
        }

        IniFile::IniFile(std::filesystem::path file_path)
        {
            if (file_path.empty() || !std::filesystem::exists(file_path))
                throw std::filesystem::filesystem_error{ "Invalid file path", std::error_code {} };

            std::ifstream ini_file{ file_path };
            std::stringstream ini_stream;
            ini_stream << ini_file.rdbuf();
            ini_file.close();
            std::string line;
            std::string key = IniFile::GLOBAL;
            while (std::getline(ini_stream, line)) {
                if (is_empty_or_comment(line))
                    continue;
                if (line[0] == SECTION_BEGIN) {
                    StrUtils::trim_trailing_space(line);
                    auto section_end = line.find(SECTION_END);
                    if (section_end == std::string::npos)
                        throw std::logic_error{ "Invalid IniFile format. Attempting to read " + line };
                    key = line.substr(1, section_end - 1);  // -1 starting from 1 ;)
                    StrUtils::trim_spaces(key);
                }
                else
                    ini_stream.seekg(-1 * ((int)line.size() + 1), std::ios_base::cur);  // TODO: hacky as hell
                    // read values
                while (ini_stream.peek() != SECTION_BEGIN &&  std::getline(ini_stream, line)) {
                    if (is_empty_or_comment(line))
                        continue;
                    auto sep = line.find(SEPARATOR);
                    if(sep == std::string::npos) {
                        StrUtils::trim_spaces(line);
                        _values[key][line] = "";
                    }
                    else {
                        auto sub_key = line.substr(0, sep);
                        StrUtils::trim_spaces(sub_key);
                        auto sub_value = line.substr(sep + 1);
                        StrUtils::trim_spaces(sub_value);
                        _values[key][sub_key] = sub_value;
                    }
                }
            }
        }
    }
}
