#pragma once
#include "header_libs/json.hpp"
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <stack>

namespace parser{
    class BencodeParser{

    public:
        nlohmann::json parse(const std::string& str);

        std::pair<nlohmann::json, size_t> getElement(const std::string& str);

        std::pair<nlohmann::json, size_t>  getString(const std::string& str);

        std::pair<nlohmann::json, size_t> getInteger(const std::string& str);

        std::pair<nlohmann::json, size_t>  getList(const std::string& str);

        std::pair<nlohmann::json, size_t>  getDict(const std::string& str);

    private:
        static bool isDict(char ch){
            return  ch == 'd';
        }
        static bool isString(char ch){
            return isdigit(ch);
        }

        static bool isInteger(char ch){
            return isdigit(ch);
        }

        static bool isList(char ch){
            return isdigit(ch);
        }
    };
}