/**
 * @file test_utilfunctions.cpp
 * @author Simon Cahill (s.cahill@procyon-systems.de)
 * @brief Contains the unit tests for the utility functions in the gzipp library.
 * @version 0.1
 * @date 2025-04-25
 * 
 * @copyright Copyright (c) 2025 Procyon Systems All Rights Reserved
 * @license BSD-2-Clause
 */

#include <gtest/gtest.h>

#include <gzipp/gzipp.hpp>

#include <string>
#include <vector>

using std::string;
using std::string_view;
using std::vector;

TEST(UtilFunctionsTest, IsCompressed_ExpectFalse) {
    constexpr auto uncompressedData = "Hello, World!";
    EXPECT_FALSE(gzipp::isCompressed(uncompressedData, 13));
}

TEST(UtilFunctionsTest, IsCompressed_ExpectTrue_Uint8Arr) {
    constexpr const uint8_t compressedData[] = {
        0x1f, 0x8b, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xf3, 0x48, 0xcd, 0xc9, 0xc9, 0xd7,
        0x51, 0x08, 0xcf, 0x2f, 0xca, 0x49, 0x51, 0x04, 0x00, 0xd0, 0xc3, 0x4a, 0xec, 0x0d, 0x00, 0x00,
        0x00
    };
    EXPECT_TRUE(gzipp::isCompressed(compressedData, sizeof(compressedData)));
}

TEST(UtilFunctionsTest, IsCompressed_ExpectTrue_String) {
    constexpr const string_view uncompressed{"Hello, World!"};
    const string compressedData = gzipp::compress(uncompressed);

    EXPECT_TRUE(gzipp::isCompressed(compressedData));
}