#include <gtest/gtest.h>
#include "core/Timer.h"
#include <thread>

using namespace Forge;

TEST(TimerTest, ElapsedTime) {
    Timer timer;
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    float elapsed = timer.Elapsed();
    EXPECT_GE(elapsed, 0.045f); // Allow some tolerance for sleep inaccuracy
    EXPECT_LT(elapsed, 0.1f);
}

TEST(TimerTest, Reset) {
    Timer timer;
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    timer.Reset();
    float elapsed = timer.Elapsed();
    EXPECT_LT(elapsed, 0.01f);
}

TEST(StopwatchTest, InitialState) {
    Stopwatch sw;
    EXPECT_FALSE(sw.IsPaused());
    EXPECT_GE(sw.Elapsed(), 0.0f);
}

TEST(StopwatchTest, PauseResume) {
    Stopwatch sw;
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    sw.Pause();
    float pausedTime = sw.Elapsed();
    EXPECT_TRUE(sw.IsPaused());

    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    EXPECT_FLOAT_EQ(sw.Elapsed(), pausedTime); // Time should not change while paused

    sw.Resume();
    EXPECT_FALSE(sw.IsPaused());
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    float finalTime = sw.Elapsed();
    EXPECT_GE(finalTime, pausedTime + 0.045f);
}

TEST(StopwatchTest, Reset) {
    Stopwatch sw;
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    sw.Pause();
    sw.Reset();
    EXPECT_FALSE(sw.IsPaused());
    EXPECT_LT(sw.Elapsed(), 0.01f);
}