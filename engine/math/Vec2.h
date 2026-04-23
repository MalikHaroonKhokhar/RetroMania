#pragma once

#include <cmath>
#include <iostream>

namespace Forge {

/**
 * @brief A 2D vector class.
 */
struct Vec2 {
    float x = 0.0f;
    float y = 0.0f;

    Vec2() = default;
    Vec2(float x, float y) : x(x), y(y) {}

    Vec2 operator+(const Vec2& other) const { return {x + other.x, y + other.y}; }
    Vec2 operator-(const Vec2& other) const { return {x - other.x, y - other.y}; }
    Vec2 operator*(float scalar) const { return {x * scalar, y * scalar}; }
    Vec2 operator/(float scalar) const { return {x / scalar, y / scalar}; }

    Vec2& operator+=(const Vec2& other) { x += other.x; y += other.y; return *this; }
    Vec2& operator-=(const Vec2& other) { x -= other.x; y -= other.y; return *this; }
    Vec2& operator*=(float scalar) { x *= scalar; y *= scalar; return *this; }
    Vec2& operator/=(float scalar) { x /= scalar; y /= scalar; return *this; }

    bool operator==(const Vec2& other) const {
        return std::abs(x - other.x) < 1e-5f && std::abs(y - other.y) < 1e-5f;
    }
    bool operator!=(const Vec2& other) const { return !(*this == other); }

    /**
     * @brief Computes the dot product of two vectors.
     */
    float dot(const Vec2& other) const { return x * other.x + y * other.y; }

    /**
     * @brief Computes the cross product (2D determinant) of two vectors.
     */
    float cross(const Vec2& other) const { return x * other.y - y * other.x; }

    /**
     * @brief Computes the squared magnitude (length) of the vector.
     */
    float lengthSquared() const { return x * x + y * y; }

    /**
     * @brief Computes the magnitude (length) of the vector.
     */
    float length() const { return std::sqrt(lengthSquared()); }

    /**
     * @brief Normalizes the vector in place. If the vector is a zero vector, it does nothing.
     */
    Vec2& normalize() {
        float lenSq = lengthSquared();
        if (lenSq > 1e-10f) {
            float invLen = 1.0f / std::sqrt(lenSq);
            x *= invLen;
            y *= invLen;
        } else {
            x = 0.0f;
            y = 0.0f;
        }
        return *this;
    }

    /**
     * @brief Returns a normalized copy of the vector.
     */
    Vec2 normalized() const {
        Vec2 v = *this;
        v.normalize();
        return v;
    }

    /**
     * @brief Linearly interpolates between this vector and another.
     */
    Vec2 lerp(const Vec2& other, float t) const {
        return {x + (other.x - x) * t, y + (other.y - y) * t};
    }

    /**
     * @brief Distance to another vector.
     */
    float distance(const Vec2& other) const {
        return (*this - other).length();
    }
};

inline Vec2 operator*(float scalar, const Vec2& vec) {
    return vec * scalar;
}

inline std::ostream& operator<<(std::ostream& os, const Vec2& vec) {
    os << "(" << vec.x << ", " << vec.y << ")";
    return os;
}

} // namespace Forge