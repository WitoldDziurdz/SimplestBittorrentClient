#pragma once
#include "header_libs/json.hpp"

namespace parser{
    class BencodeParser{

    private:
        std::string getString(const std::string& str){
            auto it_end_of_num = str.find(':');
            if(it_end_of_num == std::string::npos or it_end_of_num == 0){
                throw std::runtime_error("Invalid string to parse: " + str);
            }
            size_t nums_of_char = std::stoi(str.substr(0, it_end_of_num));
            std::string word = str.substr(it_end_of_num + 1, nums_of_char+1);
            return word;
        }

        int64_t getInteger(const std::string& str){
            //todo
        }

    };
}