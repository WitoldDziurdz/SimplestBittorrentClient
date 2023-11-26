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

        std::pair<nlohmann::json, size_t> getElement(std::string_view str);

        std::pair<nlohmann::json, size_t>  getString(std::string_view);

        std::pair<nlohmann::json, size_t> getInteger(std::string_view str);

        std::pair<nlohmann::json, size_t>  getList(std::string_view str);

        std::pair<nlohmann::json, size_t>  getDict(std::string_view str);

    private:
        static bool isDict(char ch){
            return  ch == 'd';
        }
        static bool isString(char ch){
            return isdigit(ch);
        }

        static bool isInteger(char ch){
            return ch == 'i';
        }

        static bool isList(char ch){
            return  ch == 'l';
        }
    };
}