#pragma once

#include <chrono>

namespace Forge {

/**
 * @brief A basic timer for tracking elapsed time.
 */
class Timer {
public:
    Timer() {
        Reset();
    }

    /**
     * @brief Resets the timer to the current time.
     */
    void Reset() {
        m_Start = std::chrono::high_resolution_clock::now();
    }

    /**
     * @brief Returns the elapsed time in seconds.
     */
    float Elapsed() const {
        return std::chrono::duration_cast<std::chrono::nanoseconds>(
                   std::chrono::high_resolution_clock::now() - m_Start)
                   .count() *
               0.001f * 0.001f * 0.001f;
    }

    /**
     * @brief Returns the elapsed time in milliseconds.
     */
    float ElapsedMillis() const {
        return Elapsed() * 1000.0f;
    }

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> m_Start;
};

/**
 * @brief A stopwatch utility with pause and resume functionality.
 */
class Stopwatch {
public:
    Stopwatch() {
        m_IsPaused = false;
        m_PausedTime = 0.0f;
        Reset();
    }

    void Reset() {
        m_Timer.Reset();
        m_IsPaused = false;
        m_PausedTime = 0.0f;
    }

    void Pause() {
        if (!m_IsPaused) {
            m_PausedTime += m_Timer.Elapsed();
            m_IsPaused = true;
        }
    }

    void Resume() {
        if (m_IsPaused) {
            m_Timer.Reset();
            m_IsPaused = false;
        }
    }

    float Elapsed() const {
        if (m_IsPaused) {
            return m_PausedTime;
        }
        return m_PausedTime + m_Timer.Elapsed();
    }

    bool IsPaused() const { return m_IsPaused; }

private:
    Timer m_Timer;
    bool m_IsPaused;
    float m_PausedTime;
};

} // namespace Forge