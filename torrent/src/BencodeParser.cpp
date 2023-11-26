//
// Created by vitold on 11/25/23.
//

#include "../BencodeParser.h"
#include <string>
#include <exception>
#include <stdexcept>
#include <iostream>
#include <string_view>

namespace parser{

    nlohmann::json BencodeParser::parse(const std::string& str){
        nlohmann::json res;
        std::string_view word = str;
        while(not word.empty()){
            auto pr = getElement(word);
            res.push_back(pr.first);
            word = word.substr(pr.second);
        }
        if(not word.empty()){
            throw std::invalid_argument("Invalid string to parse: " + str);
        }
        std::cout << "torrent json: " << res << std::endl;
        return res;
    }

    std::pair<nlohmann::json, size_t> BencodeParser::getElement(std::string_view str){
        if(isString(str[0])){
            return getString(str);
        } else if(isInteger(str[0])){
            return getInteger(str);
        }else if(isList(str[0])){
            return getList(str);
        } else if(isDict(str[0])){
            return getDict(str);
        } else{
            throw std::invalid_argument("Invalid string to parse: " + std::string(str));
        }
    }

    std::pair<nlohmann::json, size_t>  BencodeParser::getList(std::string_view str){
        std::vector<nlohmann::json> res;
        if(str.size() < 2){
            throw std::invalid_argument("Invalid string to parse: " + std::string(str));
        }
        if(str[0] == 'l' and str[1] == 'e'){
            return {nlohmann::json(res), 2};
        }
        auto word = str.substr(1);
        size_t index = 1;
        while((not word.empty())){
            if(word[0] == 'e'){
                break;
            }
            auto pr = getElement(word);
            word = word.substr(pr.second); // Corrected line
            index += pr.second;
            res.push_back(pr.first);
        }
        if(word[0] != 'e'){
            throw std::invalid_argument("Invalid string to parse: " + std::string(str));
        }
        return {nlohmann::json(res), index + 1};
    }

    std::pair<nlohmann::json, size_t>  BencodeParser::getDict(std::string_view str){
        std::unordered_map<nlohmann::json,nlohmann::json> res;
        if(str.size() < 2){
            throw std::invalid_argument("Invalid string to parse: " + std::string(str));
        }
        if(str[0] == 'd' and str[1] == 'e'){
            return {nlohmann::json(res), 2};
        }
        auto word = str.substr(1);
        size_t index = 1;
        while(not word.empty()){
            if(word[0] == 'e'){
                break;
            }
            auto pr1 = getElement(word);
            index += pr1.second;
            word = word.substr(pr1.second);
            auto pr2 = getElement(word);
            index += pr2.second;
            word = word.substr(pr2.second);
            res.insert({pr1.first.get<std::string>(), pr2.first});
        }
        if(word[0] != 'e'){
            throw std::invalid_argument("Invalid string to parse: " + std::string(str));
        }
        return {nlohmann::json(res), index + 1};
    }

    std::pair<nlohmann::json, size_t> BencodeParser::getString(std::string_view str){
        auto finish_it = str.find(':');
        if(finish_it == std::string_view::npos or finish_it == 0){
            throw std::invalid_argument("Invalid string to parse: " + std::string(str));
        }
        size_t nums_of_char = std::stoi(std::string(str.substr(0, finish_it)));
        auto start_it = finish_it + 1;
        finish_it = start_it + nums_of_char;
        if(str.size() < finish_it){
            throw std::invalid_argument("Invalid string to parse: " + std::string(str));
        }
        std::string_view word = str.substr(start_it, finish_it - start_it);
        return {nlohmann::json(std::string(word)), finish_it};
    }

    std::pair<nlohmann::json, size_t> BencodeParser::getInteger(std::string_view str){
        auto start_it = str.find('i');
        if(start_it == std::string_view::npos){
            throw std::invalid_argument("Invalid string to parse: " + std::string(str));
        }
        ++start_it;
        auto finish_it = start_it;
        if(str[finish_it] == '-'){
            ++finish_it;
        }
        while(isdigit(str[finish_it])){
            ++finish_it;
        }
        if(str[finish_it] != 'e'){
            throw std::invalid_argument("Invalid string to parse: " + std::string(str));
        }
        if(finish_it == std::string_view::npos or finish_it == 0 or start_it == finish_it){
            throw std::invalid_argument("Invalid string to parse: " + std::string(str));
        }
        std::string_view num = str.substr(start_it, finish_it - start_it);
        if(num.size() > 1 && num[0] == '0'){
            throw std::invalid_argument("Invalid string to parse: " + std::string(str));
        }
        return {nlohmann::json(std::stoll(std::string(num))), finish_it + 1};
    }
}