#ifndef CANH_TIMER_H
#define CANH_TIMER_H

#include <chrono>
#include <cstdint>

class Timer
{
public:
    typedef uint32_t Sec;
    Timer() : m_Running(false) {};

    void start();
    void stop();
    Sec getSecond() const;

private:
    bool m_Running;
    std::chrono::time_point<std::chrono::system_clock> m_StartTime;
    std::chrono::time_point<std::chrono::system_clock> m_StopTime;
};
#endif
