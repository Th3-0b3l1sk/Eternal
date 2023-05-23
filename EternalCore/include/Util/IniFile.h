#pragma once
#include <filesystem>
#include <string>
#include <unordered_map>
#include <type_traits>

namespace Eternal
{
    namespace Util
    {
        class IniFile
        {
        public:
            IniFile(std::filesystem::path file_path);

        public:
            template<typename T>
            T get(const std::string& root, const std::string& key)
            {
                check_value_throw_not_there(root, key);

                std::string value = _values[root][key];
                if constexpr (std::is_integral<T>::value) {
                    return static_cast<T>(std::stoll(value));
                }

                if constexpr (std::is_floating_point<T>::value)
                {
                    return static_cast<T>(std::stod(value));
                }

                throw std::logic_error{ "Invalid conversion type" };
                return T{};
            }

            // specialization of the default type
            std::string& get(const std::string& root, const std::string& key)
            {
                check_value_throw_not_there(root, key);

                return _values[root][key];
            }
            
            std::unordered_map<std::string,
                std::unordered_map<std::string, std::string>>::const_iterator cbegin() const { return _values.cbegin(); }

            std::unordered_map<std::string,
                std::unordered_map<std::string, std::string>>::const_iterator cend() const { return _values.cend(); }


            std::size_t entries() const { return _values.size(); }
        public:
            static const std::string GLOBAL;

        private:
            void check_value_throw_not_there(const std::string& root, const std::string& key)
            {
                if (_values.find(root) == _values.end() || _values[root].find(key) == _values[root].end())
                    throw std::logic_error{ " The value doesn't exist" };
            }

        private:
            std::unordered_map<std::string, 
                std::unordered_map<std::string, std::string>> _values;
        };
   
    }
}
