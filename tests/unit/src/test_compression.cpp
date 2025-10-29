/**
 * @file test_compression.cpp
 * @author Simon Cahill (s.cahill@procyon-systems.de)
 * @brief Contains the compression unit tests for the gzipp library.
 * @version 0.1
 * @date 2025-04-25
 * 
 * @copyright Copyright (c) 2025 Procyon Systems All Rights Reserved
 * @license BSD-2-Clause
 */

#include <gtest/gtest.h>

#include <gzipp/gzipp.hpp>

TEST(CompressionTest, CompressString) {
    std::string input = "Hello, World!";
    std::string compressed = gzipp::compress(input);
    
    EXPECT_FALSE(compressed.empty());
    EXPECT_NE(input, compressed);
    EXPECT_TRUE(gzipp::isCompressed(compressed));
}

TEST(CompressionTest, DecompressString) {
    std::string input = "Hello, World!";
    std::string compressed = gzipp::compress(input);
    std::string decompressed = gzipp::decompress(compressed);
    
    EXPECT_EQ(input, decompressed);
}