#pragma once

#include "ecs/ECS.h"
#include "ecs/PhysicsComponents.h"
#include <cmath>

namespace Forge {

class PhysicsSystem {
public:
    static constexpr float GRAVITY = 9.8f * 10.0f; // Scale up gravity for 2D feel

    static void Update(Registry& registry, float dt) {
        // 1. Integrate Physics (Apply forces and velocity)
        auto bodies = registry.View<Transform, Rigidbody>();
        for (Entity e : bodies) {
            auto& rb = registry.GetComponent<Rigidbody>(e);
            if (rb.IsStatic) continue;

            auto& transform = registry.GetComponent<Transform>(e);

            // Apply gravity
            rb.Velocity.y += GRAVITY * rb.GravityScale * dt;

            // Apply acceleration
            rb.Velocity.x += rb.Acceleration.x * dt;
            rb.Velocity.y += rb.Acceleration.y * dt;

            // Update position
            transform.x += rb.Velocity.x * dt;
            transform.y += rb.Velocity.y * dt;
        }

        // 2. Simple Collision Detection & Resolution (AABB vs AABB for now)
        // We do a naive O(N^2) check. For a real engine, we'd use a quadtree or spatial hash.
        auto boxColliders = registry.View<Transform, BoxCollider>();

        for (size_t i = 0; i < boxColliders.size(); ++i) {
            for (size_t j = i + 1; j < boxColliders.size(); ++j) {
                Entity e1 = boxColliders[i];
                Entity e2 = boxColliders[j];

                auto& t1 = registry.GetComponent<Transform>(e1);
                auto& bc1 = registry.GetComponent<BoxCollider>(e1);

                auto& t2 = registry.GetComponent<Transform>(e2);
                auto& bc2 = registry.GetComponent<BoxCollider>(e2);

                if (CheckAABBCollision(t1, bc1, t2, bc2)) {
                    if (!bc1.IsTrigger && !bc2.IsTrigger) {
                        ResolveAABBCollision(registry, e1, t1, bc1, e2, t2, bc2);
                    }
                }
            }
        }

        // Note: Circle vs Circle and AABB vs Circle can be added similarly
    }

private:
    static bool CheckAABBCollision(const Transform& t1, const BoxCollider& bc1,
                                   const Transform& t2, const BoxCollider& bc2) {
        float left1 = t1.x + bc1.Offset.x;
        float right1 = left1 + bc1.Size.x * t1.scaleX;
        float top1 = t1.y + bc1.Offset.y;
        float bottom1 = top1 + bc1.Size.y * t1.scaleY;

        float left2 = t2.x + bc2.Offset.x;
        float right2 = left2 + bc2.Size.x * t2.scaleX;
        float top2 = t2.y + bc2.Offset.y;
        float bottom2 = top2 + bc2.Size.y * t2.scaleY;

        return left1 < right2 && right1 > left2 && top1 < bottom2 && bottom1 > top2;
    }

    static void ResolveAABBCollision(Registry& registry,
                                     Entity e1, Transform& t1, const BoxCollider& bc1,
                                     Entity e2, Transform& t2, const BoxCollider& bc2) {

        bool e1HasRb = registry.HasComponent<Rigidbody>(e1);
        bool e2HasRb = registry.HasComponent<Rigidbody>(e2);

        // If neither has rigidbody or both are static, we don't resolve
        if ((!e1HasRb && !e2HasRb) ||
            (e1HasRb && registry.GetComponent<Rigidbody>(e1).IsStatic && e2HasRb && registry.GetComponent<Rigidbody>(e2).IsStatic)) {
            return;
        }

        float center1X = t1.x + bc1.Offset.x + (bc1.Size.x * t1.scaleX) / 2.0f;
        float center1Y = t1.y + bc1.Offset.y + (bc1.Size.y * t1.scaleY) / 2.0f;
        float center2X = t2.x + bc2.Offset.x + (bc2.Size.x * t2.scaleX) / 2.0f;
        float center2Y = t2.y + bc2.Offset.y + (bc2.Size.y * t2.scaleY) / 2.0f;

        float dx = center1X - center2X;
        float dy = center1Y - center2Y;

        float halfWidths = ((bc1.Size.x * t1.scaleX) + (bc2.Size.x * t2.scaleX)) / 2.0f;
        float halfHeights = ((bc1.Size.y * t1.scaleY) + (bc2.Size.y * t2.scaleY)) / 2.0f;

        float crossWidth = halfWidths * dy;
        float crossHeight = halfHeights * dx;

        // Determine resolution vector based on who is movable
        float resolveX = 0;
        float resolveY = 0;

        if (std::abs(dx) <= halfWidths && std::abs(dy) <= halfHeights) {
            float wy = halfWidths * dy;
            float hx = halfHeights * dx;

            if (wy > hx) {
                if (wy > -hx) {
                    // Top collision
                    resolveY = halfHeights - dy;
                } else {
                    // Left collision
                    resolveX = -halfWidths - dx;
                }
            } else {
                if (wy > -hx) {
                    // Right collision
                    resolveX = halfWidths - dx;
                } else {
                    // Bottom collision
                    resolveY = -halfHeights - dy;
                }
            }
        }

        // Apply resolution
        if (e1HasRb && !registry.GetComponent<Rigidbody>(e1).IsStatic) {
            t1.x += resolveX;
            t1.y += resolveY;

            auto& rb1 = registry.GetComponent<Rigidbody>(e1);
            if (resolveX != 0) rb1.Velocity.x = 0;
            if (resolveY != 0) rb1.Velocity.y = 0;
        } else if (e2HasRb && !registry.GetComponent<Rigidbody>(e2).IsStatic) {
            t2.x -= resolveX;
            t2.y -= resolveY;

            auto& rb2 = registry.GetComponent<Rigidbody>(e2);
            if (resolveX != 0) rb2.Velocity.x = 0;
            if (resolveY != 0) rb2.Velocity.y = 0;
        }
    }
};

} // namespace Forge