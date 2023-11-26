//
// Created by vitold on 11/26/23.
//

#include "Metainfo.h"

namespace torrent {

    std::ostream& operator<<(std::ostream& os, const MetainfoData& info)
    {
        os << "announce: " << info.announce << std::endl;
        os << "length: " << info.length << std::endl;
        os << "name: " << info.name << std::endl;
        os << "piece_length: " << info.piece_length << std::endl;
        return os;
    }

    bool operator==(const MetainfoData& lhs, const MetainfoData& rhs)
    {
        return std::tie(lhs.announce, lhs.length, lhs.name, lhs.piece_length) ==
               std::tie(rhs.announce, rhs.length, rhs.name, rhs.piece_length);
    }

    MetainfoData Metainfo::getInfo(std::filesystem::path file_path) {
        std::ifstream file_stream(file_path);

        if (not file_stream.is_open() or file_stream.bad()) {
            throw std::invalid_argument("Invalid file name or path: " + std::string(file_path));
        }

        std::string torrent_info((std::istreambuf_iterator<char>(file_stream)), (std::istreambuf_iterator<char>()));
        return createData(torrent_info);
    }

    MetainfoData Metainfo::createData(const std::string &str) {
        parser::BencodeParser parser;
        auto data = parser.parse(str);
        std::string announce = data[0]["announce"];
        uint64_t length = data[0]["info"]["length"];
        std::string name = data[0]["info"]["name"];
        uint64_t piece_length = data[0]["info"]["piece length"];
        //std::string pieces = data[0]["info"]["pieces"];
        return {announce, length, name, piece_length};
    }
}