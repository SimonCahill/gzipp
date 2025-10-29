/**
 * @file test_main.cpp
 * @author Simon Cahill (s.cahill@procyon-systems.de)
 * @brief Contains the main function for the unit tests.
 * @version 0.1
 * @date 2025-04-25
 * 
 * @copyright Copyright (c) 2025 Procyon Systems All Rights Reserved
 * @license BSD-2-Clause
 */

#include <gtest/gtest.h>

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}