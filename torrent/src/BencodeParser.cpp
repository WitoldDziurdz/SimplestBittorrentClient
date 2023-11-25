//
// Created by vitold on 11/25/23.
//

#include "../BencodeParser.h"
#include <string>
#include <exception>
#include <stdexcept>

namespace parser{

    nlohmann::json BencodeParser::parse(const std::string& str){
        nlohmann::json res;
        std::string word = str;
        while(not word.empty()){
            auto pr = getElement(word);
            res.push_back(pr.first);
            word = word.substr(pr.second);
        }
        return res;
    }

    std::pair<nlohmann::json, size_t> BencodeParser::getElement(const std::string& str){
        if(isString(str[0])){
            return getString(str);
        } else if(isInteger(str[0])){
            return getInteger(str);
        }else if(isList(str[0])){
            return getList(str);
        } else if(isDict(str[0])){
            return getDict(str);
        } else{
            throw std::invalid_argument("Invalid string to parse: " + str);
        }
    }

    std::pair<nlohmann::json, size_t>  BencodeParser::getList(const std::string& str){
        std::vector<nlohmann::json> res;
        if(str.size() < 2){
            throw std::invalid_argument("Invalid string to parse: " + str);
        }
        if(str[0] == 'l' and str[1] == 'e'){
            return {nlohmann::json(res), 2};
        }
        auto word = str.substr(1);
        size_t index = 0;
        while(not word.empty() and word[0] != 'e'){
            auto pr = getElement(word);
            word = word.substr(pr.second); // Corrected line
            index += pr.second;
            res.push_back(pr.first);
        }

        return {nlohmann::json(res), index + 1};
    }

    std::pair<nlohmann::json, size_t>  BencodeParser::getDict(const std::string& str){
        std::unordered_map<nlohmann::json,nlohmann::json> res;
        if(str.size() < 2){
            throw std::invalid_argument("Invalid string to parse: " + str);
        }
        if(str[0] == 'd' and str[1] == 'e'){
            return {nlohmann::json(res), 2};
        }
        auto word = str.substr(1);
        size_t index = 0;
        while(not word.empty() and word[0] != 'e'){
            auto pr1 = getString(word);
            index += pr1.second;
            word = word.substr(pr1.second);
            auto pr2 = getElement(word);
            index += pr2.second;
            word = word.substr(pr2.second);
            res.insert({pr1.first.get<std::string>(), pr2.first});
        }
        return {nlohmann::json(res), index + 1};
    }

    std::pair<nlohmann::json, size_t> BencodeParser::getString(const std::string& str){
        auto finish_it = str.find(':');
        if(finish_it == std::string::npos or finish_it == 0){
            throw std::invalid_argument("Invalid string to parse: " + str);
        }
        size_t nums_of_char = std::stoi(str.substr(0, finish_it));
        auto start_it = finish_it + 1;
        finish_it = start_it + nums_of_char;
        if(str.size() < finish_it){
            throw std::invalid_argument("Invalid string to parse: " + str);
        }
        std::string word = str.substr(start_it, finish_it - start_it);
        return {nlohmann::json(word), finish_it};
    }

    std::pair<nlohmann::json, size_t> BencodeParser::getInteger(const std::string& str){
        auto start_it = str.find('i');
        if(start_it == std::string::npos){
            throw std::invalid_argument("Invalid string to parse: " + str);
        }
        ++start_it;
        auto finish_it = str.find('e');
        if(finish_it == std::string::npos or finish_it == 0 or start_it == finish_it){
            throw std::invalid_argument("Invalid string to parse: " + str);
        }
        std::string num = str.substr(start_it, finish_it - start_it);
        if(num.size() > 1 && num[0] == '0'){
            throw std::invalid_argument("Invalid string to parse: " + str);
        }
        return {nlohmann::json(std::stoll(num)), finish_it};
    }
}