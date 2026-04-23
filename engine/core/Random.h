#pragma once

#include <random>

namespace Forge {

/**
 * @brief Utility for generating random numbers.
 */
class Random {
public:
    static void Init() {
        s_RandomEngine.seed(std::random_device()());
    }

    /**
     * @brief Returns a random float between [0.0, 1.0].
     */
    static float Float() {
        return (float)s_Distribution(s_RandomEngine) / (float)std::mt19937::max();
    }

    /**
     * @brief Returns a random float between [min, max].
     */
    static float Float(float min, float max) {
        return min + Float() * (max - min);
    }

    /**
     * @brief Returns a random integer between [min, max].
     */
    static int Int(int min, int max) {
        std::uniform_int_distribution<int> dist(min, max);
        return dist(s_RandomEngine);
    }

private:
    inline static std::mt19937 s_RandomEngine;
    inline static std::uniform_int_distribution<std::mt19937::result_type> s_Distribution;
};

} // namespace Forge