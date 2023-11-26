#pragma once
#include <filesystem>
#include <iostream>
#include <fstream>
#include <tuple>
#include "BencodeParser.h"

namespace torrent
{
    struct MetainfoData{
        std::string announce;
        uint64_t length;
        std::string name;
        uint64_t piece_length;
    };

    std::ostream& operator<<(std::ostream& os, const MetainfoData& info);

    bool operator==(const MetainfoData& lhs, const MetainfoData& rhs);

    class Metainfo {
    public:
        MetainfoData getInfo(std::filesystem::path file_path);

    private:
        MetainfoData createData(const std::string& str);
    };
}
