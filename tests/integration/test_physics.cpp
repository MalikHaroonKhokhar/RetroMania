#include <gtest/gtest.h>
#include "ecs/ECS.h"
#include "ecs/PhysicsComponents.h"
#include "ecs/PhysicsSystems.h"

using namespace Forge;

TEST(PhysicsTest, RigidbodyIntegration) {
    Registry registry;
    Entity e = registry.CreateEntity();

    registry.AddComponent(e, Transform{0.0f, 0.0f});

    Rigidbody rb;
    rb.Velocity = {10.0f, 0.0f}; // Move right
    rb.GravityScale = 0.0f;      // Turn off gravity for this test
    registry.AddComponent(e, rb);

    PhysicsSystem::Update(registry, 1.0f); // Simulate 1 second

    auto& t = registry.GetComponent<Transform>(e);
    EXPECT_FLOAT_EQ(t.x, 10.0f);
    EXPECT_FLOAT_EQ(t.y, 0.0f);
}

TEST(PhysicsTest, GravityApplication) {
    Registry registry;
    Entity e = registry.CreateEntity();

    registry.AddComponent(e, Transform{0.0f, 0.0f});

    Rigidbody rb;
    rb.GravityScale = 1.0f;
    registry.AddComponent(e, rb);

    PhysicsSystem::Update(registry, 1.0f);

    auto& rbRet = registry.GetComponent<Rigidbody>(e);
    auto& t = registry.GetComponent<Transform>(e);

    // Velocity should be increased by gravity (98.0)
    EXPECT_FLOAT_EQ(rbRet.Velocity.y, 98.0f);

    // Position should be updated by velocity (in one big step)
    EXPECT_FLOAT_EQ(t.y, 98.0f);
}

TEST(PhysicsTest, StaticRigidbodyDoesNotMove) {
    Registry registry;
    Entity e = registry.CreateEntity();

    registry.AddComponent(e, Transform{0.0f, 0.0f});

    Rigidbody rb;
    rb.Velocity = {10.0f, 10.0f};
    rb.IsStatic = true;
    registry.AddComponent(e, rb);

    PhysicsSystem::Update(registry, 1.0f);

    auto& t = registry.GetComponent<Transform>(e);
    EXPECT_FLOAT_EQ(t.x, 0.0f);
    EXPECT_FLOAT_EQ(t.y, 0.0f);
}

TEST(PhysicsTest, AABBCollisionResolution) {
    Registry registry;

    // Dynamic entity moving right
    Entity e1 = registry.CreateEntity();
    registry.AddComponent(e1, Transform{0.0f, 0.0f});
    Rigidbody rb1;
    rb1.Velocity = {10.0f, 0.0f};
    rb1.GravityScale = 0.0f;
    registry.AddComponent(e1, rb1);

    BoxCollider bc1;
    bc1.Size = {10.0f, 10.0f};
    registry.AddComponent(e1, bc1);

    // Static entity in the way
    Entity e2 = registry.CreateEntity();
    registry.AddComponent(e2, Transform{5.0f, 0.0f}); // Overlaps with e1 initially

    Rigidbody rb2;
    rb2.IsStatic = true;
    registry.AddComponent(e2, rb2);

    BoxCollider bc2;
    bc2.Size = {10.0f, 10.0f};
    registry.AddComponent(e2, bc2);

    // Run physics
    PhysicsSystem::Update(registry, 0.1f); // 0.1s step

    auto& t1 = registry.GetComponent<Transform>(e1);
    auto& rb1_after = registry.GetComponent<Rigidbody>(e1);

    // The collision resolver should push e1 to the left (t1.x becomes -5)
    EXPECT_LT(t1.x, 5.0f);
    // Since it collided horizontally, its velocity x should be 0
    EXPECT_FLOAT_EQ(rb1_after.Velocity.x, 0.0f);
}