#include <gtest/gtest.h>
#include <iostream>
#include <filesystem>
#include "Metainfo.h"

TEST(MetainfoTest, getInfoValidFile) {
    torrent::Metainfo info;
    std::filesystem::path path("tests/test_data/ubuntu-test.torrent");
    torrent::MetainfoData expected_info{
            "https://torrent.ubuntu.com/announce",
            4071903232,
            "ubuntu-22.10-desktop-amd64.iso",
            262144,

    };
    auto meta = info.getInfo(path);
    EXPECT_EQ(meta, expected_info);
}

TEST(MetainfoTest, getInfoInvalidFile) {
    torrent::Metainfo info;
    std::filesystem::path path("fake_file.torrent");
    EXPECT_THROW(info.getInfo(path), std::invalid_argument);
}