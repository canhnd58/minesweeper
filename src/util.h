#ifndef CANH_UTIL_H
#define CANH_UTIL_H

#define CANH_DEBUG 1

#if (CANH_DEBUG == 1)
#   include <iostream>
#   include <cassert>
#   define LOG(m) std::cerr << m << std::endl
#   define ASSERT(exp) assert(exp)
#else
#   define LOG(m)
#   define ASSERT(exp)
#endif

#include <chrono>
#include <random>
#include <cstdint>

class Util
{
public:
    static std::default_random_engine getRNG() {
        return std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count());
    }
};

#endif
