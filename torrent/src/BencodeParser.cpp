//
// Created by vitold on 11/25/23.
//

#include "../BencodeParser.h"
#include <string>
#include <exception>
#include <stdexcept>

namespace parser{
    std::string BencodeParser::getString(const std::string& str){
        auto it_end_of_num = str.find(':');
        if(it_end_of_num == std::string::npos or it_end_of_num == 0){
            throw std::invalid_argument("Invalid string to parse: " + str);
        }
        size_t nums_of_char = std::stoi(str.substr(0, it_end_of_num));
        ++it_end_of_num;
        std::string hole_str = str.substr(it_end_of_num);
        if(hole_str.size() < nums_of_char){
            throw std::invalid_argument("Invalid string to parse: " + str);
        }
        std::string word = str.substr(it_end_of_num, it_end_of_num + nums_of_char);
        return word;
    }

    int64_t BencodeParser::getInteger(const std::string& str){
        auto start_it = str.find('i');
        if(start_it == std::string::npos){
            throw std::invalid_argument("Invalid string to parse: " + str);
        }
        ++start_it;
        auto finish_it = str.find('e');
        if(finish_it == std::string::npos or finish_it == 0 or start_it == finish_it){
            throw std::invalid_argument("Invalid string to parse: " + str);
        }
        if(finish_it < str.size() - 1){
            throw std::invalid_argument("Invalid string to parse: " + str);
        }
        --finish_it;
        std::string num = str.substr(start_it, finish_it);
        if(num.size() > 1 && num[0] == '0'){
            throw std::invalid_argument("Invalid string to parse: " + str);
        }
        return std::stoll(num);
    }
}