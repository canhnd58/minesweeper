#ifndef CANH_UTIL_H
#define CANH_UTIL_H

#define CANH_DEBUG 1

#if (CANH_DEBUG == 1)
#   include <iostream>
#   include <cassert>
#   define LOG(m) do { std::cout << m << std::endl; } while (0)
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
    struct Rect
    {
        int32_t x, y, w, h;
    };

    struct Color
    {
        uint8_t r, g, b, a;

        Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
            : r(r), g(g), b(b), a(a)
        {
        }

        Color(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b), a(255) {}
        Color() : r(0), g(0), b(0), a(255) {}
    };

    Util(const Util &) = delete;
    void operator=(const Util &) = delete;

    static std::default_random_engine getRNG() { return getInstance().m_rng; }

private:
    static Util& getInstance()
    {
        static Util ins;
        return ins;
    }

    Util()
        : m_rng(std::chrono::system_clock::now().time_since_epoch().count())
    {
    }

    std::default_random_engine m_rng;
};

#endif
