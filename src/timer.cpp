#include "timer.h"

#include <chrono>

void Timer::start()
{
    m_StartTime = std::chrono::system_clock::now();
    m_Running = true;
}

void Timer::stop()
{
    if (m_Running)
    {
        m_StopTime = std::chrono::system_clock::now();
        m_Running = false;
    }
}

Timer::Sec Timer::getSecond() const
{
    std::chrono::seconds sec;
    if (m_Running)
    {
        sec = std::chrono::duration_cast<std::chrono::seconds>(
            std::chrono::system_clock::now() - m_StartTime);
    }
    else
    {
        sec = std::chrono::duration_cast<std::chrono::seconds>(
            m_StopTime - m_StartTime);
    }
    return static_cast<Sec>(sec.count());
}
