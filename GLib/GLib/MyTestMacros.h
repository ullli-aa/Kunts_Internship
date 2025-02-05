#pragma once

#include <iostream>
#include <stdexcept>
#include <cmath>

#define GREEN "\033[32m"
#define RED "\033[31m"
#define RESET "\033[0m"

#define MY_ASSERT_DOUBLE_EQ(expected, actual) \
    if (std::fabs(expected - actual) > 1e-9) { \
        std::cerr << RED << "Assertion failed: " << #expected << " == " << #actual \
                  << " at " << __FILE__ << ":" << __LINE__ << RESET << std::endl; \
        throw std::runtime_error("Test failed!"); \
    }

#define MY_ASSERT_EQ(expected, actual) \
    if ((expected) != (actual)) { \
        std::cerr << RED << "Assertion failed: " << #expected << " == " << #actual \
                  << " at " << __FILE__ << ":" << __LINE__ << RESET << std::endl; \
        throw std::runtime_error("Test failed!"); \
    }

#define MY_ASSERT_VEC_EQ(expected, actual) \
    MY_ASSERT_DOUBLE_EQ(expected.X(), actual.X()); \
    MY_ASSERT_DOUBLE_EQ(expected.Y(), actual.Y()); \
    MY_ASSERT_DOUBLE_EQ(expected.Z(), actual.Z());

#define MY_ASSERT_TRUE(condition) \
    if (!(condition)) { \
        std::cerr << RED << "Assertion failed: " << #condition << " is false but should be true " \
                  << " at " << __FILE__ << ":" << __LINE__ << RESET << std::endl; \
        throw std::runtime_error("Test failed!"); \
    }

#define MY_ASSERT_FALSE(condition) \
    if (condition) { \
        std::cerr << RED << "Assertion failed: " << #condition << " is true but should be false " \
                  << " at " << __FILE__ << ":" << __LINE__ << RESET << std::endl; \
        throw std::runtime_error("Test failed!"); \
    }

#define RUN_TEST(test_name) \
    try { \
        test_name(); \
        std::cout << #test_name << GREEN << " OK" << RESET << std::endl; \
    } catch (const std::runtime_error& e) { \
        std::cerr << #test_name << RED << " FAILED " << RESET << e.what() << std::endl; \
    }