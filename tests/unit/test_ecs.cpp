#include <gtest/gtest.h>
#include "ecs/ECS.h"

using namespace Forge;

struct Position {
    float x, y;
};

struct Velocity {
    float dx, dy;
};

TEST(ECSTest, CreateAndDestroyEntity) {
    Registry registry;
    Entity e1 = registry.CreateEntity();
    Entity e2 = registry.CreateEntity();

    EXPECT_NE(e1, e2);
    EXPECT_NE(e1, NULL_ENTITY);
    EXPECT_NE(e2, NULL_ENTITY);

    registry.DestroyEntity(e1);
}

TEST(ECSTest, AddAndGetComponent) {
    Registry registry;
    Entity e = registry.CreateEntity();

    Position p = {10.0f, 20.0f};
    registry.AddComponent(e, p);

    EXPECT_TRUE(registry.HasComponent<Position>(e));

    Position& retrieved = registry.GetComponent<Position>(e);
    EXPECT_FLOAT_EQ(retrieved.x, 10.0f);
    EXPECT_FLOAT_EQ(retrieved.y, 20.0f);
}

TEST(ECSTest, RemoveComponent) {
    Registry registry;
    Entity e = registry.CreateEntity();

    registry.AddComponent(e, Position{0.0f, 0.0f});
    EXPECT_TRUE(registry.HasComponent<Position>(e));

    registry.RemoveComponent<Position>(e);
    EXPECT_FALSE(registry.HasComponent<Position>(e));
}

TEST(ECSTest, EntityDestroyedCleansUpComponents) {
    Registry registry;
    Entity e = registry.CreateEntity();

    registry.AddComponent(e, Position{1.0f, 1.0f});
    registry.DestroyEntity(e);

    EXPECT_FALSE(registry.HasComponent<Position>(e));
}

TEST(ECSTest, ViewSingleComponent) {
    Registry registry;
    Entity e1 = registry.CreateEntity();
    Entity e2 = registry.CreateEntity();
    Entity e3 = registry.CreateEntity();

    registry.AddComponent(e1, Position{1.0f, 1.0f});
    registry.AddComponent(e2, Position{2.0f, 2.0f});

    auto entities = registry.View<Position>();
    EXPECT_EQ(entities.size(), 2);

    bool hasE1 = false, hasE2 = false, hasE3 = false;
    for(auto e : entities) {
        if (e == e1) hasE1 = true;
        if (e == e2) hasE2 = true;
        if (e == e3) hasE3 = true;
    }

    EXPECT_TRUE(hasE1);
    EXPECT_TRUE(hasE2);
    EXPECT_FALSE(hasE3);
}

TEST(ECSTest, ViewMultipleComponents) {
    Registry registry;
    Entity e1 = registry.CreateEntity();
    Entity e2 = registry.CreateEntity();

    registry.AddComponent(e1, Position{1.0f, 1.0f});
    registry.AddComponent(e1, Velocity{1.0f, 1.0f});

    registry.AddComponent(e2, Position{2.0f, 2.0f});

    auto entities = registry.View<Position, Velocity>();
    EXPECT_EQ(entities.size(), 1);
    EXPECT_EQ(entities[0], e1);
}

TEST(ECSTest, SystemIterationExample) {
    Registry registry;
    Entity e = registry.CreateEntity();
    registry.AddComponent(e, Position{10.0f, 10.0f});
    registry.AddComponent(e, Velocity{5.0f, 5.0f});

    // Simulate a system update
    for (auto entity : registry.View<Position, Velocity>()) {
        auto& pos = registry.GetComponent<Position>(entity);
        auto& vel = registry.GetComponent<Velocity>(entity);
        pos.x += vel.dx;
        pos.y += vel.dy;
    }

    auto& pos = registry.GetComponent<Position>(e);
    EXPECT_FLOAT_EQ(pos.x, 15.0f);
    EXPECT_FLOAT_EQ(pos.y, 15.0f);
}