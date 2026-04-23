#include <gtest/gtest.h>
#include "ecs/ECS.h"
#include "ecs/RenderComponents.h"
#include "ecs/RenderSystems.h"

using namespace Forge;

// Mock Texture for testing without initializing SDL Window
class MockTexture : public Texture {
public:
    MockTexture() : Texture(nullptr, 100, 100) {}
};

TEST(RenderingECSTest, AddComponents) {
    Registry registry;
    Entity e = registry.CreateEntity();

    auto mockTex = std::make_shared<MockTexture>();
    registry.AddComponent(e, Transform{});
    registry.AddComponent(e, Sprite{mockTex});
    registry.AddComponent(e, Animator{});
    registry.AddComponent(e, Camera{});

    EXPECT_TRUE(registry.HasComponent<Transform>(e));
    EXPECT_TRUE(registry.HasComponent<Sprite>(e));
    EXPECT_TRUE(registry.HasComponent<Animator>(e));
    EXPECT_TRUE(registry.HasComponent<Camera>(e));
}

TEST(RenderingECSTest, AnimationSystemUpdatesFrame) {
    Registry registry;
    Entity e = registry.CreateEntity();

    auto mockTex = std::make_shared<MockTexture>();
    registry.AddComponent(e, Sprite{mockTex});

    Animator anim;
    anim.FrameDuration = 0.1f;
    anim.FrameCount = 4;
    anim.FrameWidth = 32;
    anim.FrameHeight = 32;
    anim.IsPlaying = true;
    registry.AddComponent(e, anim);

    // Initial state
    AnimationSystem::Update(registry, 0.05f); // Halfway to next frame
    EXPECT_EQ(registry.GetComponent<Animator>(e).CurrentFrame, 0);

    // Cross frame boundary
    AnimationSystem::Update(registry, 0.06f); // Total 0.11s > 0.1s
    EXPECT_EQ(registry.GetComponent<Animator>(e).CurrentFrame, 1);

    // Check if sprite source rect was updated
    auto& sprite = registry.GetComponent<Sprite>(e);
    EXPECT_TRUE(sprite.UseSourceRect);
    EXPECT_EQ(sprite.SourceRect.x, 32); // Frame 1 * width 32
}

TEST(RenderingECSTest, AnimationSystemLoops) {
    Registry registry;
    Entity e = registry.CreateEntity();

    auto mockTex = std::make_shared<MockTexture>();
    registry.AddComponent(e, Sprite{mockTex});

    Animator anim;
    anim.FrameDuration = 0.1f;
    anim.FrameCount = 2;
    anim.FrameWidth = 32;
    anim.FrameHeight = 32;
    anim.Loop = true;
    registry.AddComponent(e, anim);

    AnimationSystem::Update(registry, 0.25f); // 2.5 frames
    EXPECT_EQ(registry.GetComponent<Animator>(e).CurrentFrame, 0); // Looped around
}