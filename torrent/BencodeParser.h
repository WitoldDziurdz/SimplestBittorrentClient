#pragma once
#include "header_libs/json.hpp"

namespace parser{
    class BencodeParser{

    public:
        std::string getString(const std::string& str);

        int64_t getInteger(const std::string& str);
    private:
    };
}