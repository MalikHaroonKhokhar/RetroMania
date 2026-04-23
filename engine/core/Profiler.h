#pragma once

#include "core/Timer.h"
#include <string>
#include <iostream>

namespace Forge {

/**
 * @brief A basic profiler for measuring frame time and FPS.
 */
class Profiler {
public:
    Profiler() : m_FrameCount(0), m_FPS(0.0f), m_LastFrameTime(0.0f) {
        m_FpsTimer.Reset();
    }

    /**
     * @brief Call this at the start of every frame.
     */
    void BeginFrame() {
        m_FrameTimer.Reset();
    }

    /**
     * @brief Call this at the end of every frame.
     */
    void EndFrame() {
        m_LastFrameTime = m_FrameTimer.ElapsedMillis();
        m_FrameCount++;

        if (m_FpsTimer.Elapsed() >= 1.0f) {
            m_FPS = (float)m_FrameCount / m_FpsTimer.Elapsed();
            m_FrameCount = 0;
            m_FpsTimer.Reset();
        }
    }

    /**
     * @brief Returns the last frame time in milliseconds.
     */
    float GetLastFrameTime() const { return m_LastFrameTime; }

    /**
     * @brief Returns the current FPS.
     */
    float GetFPS() const { return m_FPS; }

private:
    Timer m_FrameTimer;
    Timer m_FpsTimer;
    uint32_t m_FrameCount;
    float m_FPS;
    float m_LastFrameTime;
};

/**
 * @brief Scope timer to easily log how long a scope took.
 */
class ProfileScope {
public:
    ProfileScope(const std::string& name) : m_Name(name) {
        m_Timer.Reset();
    }

    ~ProfileScope() {
        std::cout << "[Profile] " << m_Name << " took " << m_Timer.ElapsedMillis() << "ms\n";
    }

private:
    std::string m_Name;
    Timer m_Timer;
};

#define PROFILE_SCOPE(name) ::Forge::ProfileScope profileScope##__LINE__(name)
#define PROFILE_FUNCTION() PROFILE_SCOPE(__FUNCTION__)

} // namespace Forge