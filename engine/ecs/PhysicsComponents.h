#pragma once

#include "math/Vec2.h"

namespace Forge {

/**
 * @brief Rigidbody component for physics simulation.
 */
struct Rigidbody {
    Vec2 Velocity = {0.0f, 0.0f};
    Vec2 Acceleration = {0.0f, 0.0f};
    float Mass = 1.0f;
    float GravityScale = 1.0f;
    bool IsStatic = false;

    Rigidbody() = default;
};

/**
 * @brief Abstract base class for colliders.
 */
struct Collider {
    enum class Type { AABB, Circle };
    Type ShapeType;
    bool IsTrigger = false;
    Vec2 Offset = {0.0f, 0.0f};

    Collider(Type type) : ShapeType(type) {}
};

/**
 * @brief Axis-Aligned Bounding Box (AABB) collider.
 */
struct BoxCollider : public Collider {
    Vec2 Size = {1.0f, 1.0f};

    BoxCollider() : Collider(Type::AABB) {}
};

/**
 * @brief Circle collider.
 */
struct CircleCollider : public Collider {
    float Radius = 1.0f;

    CircleCollider() : Collider(Type::Circle) {}
};

} // namespace Forge