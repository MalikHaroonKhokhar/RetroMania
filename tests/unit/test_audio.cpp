#include <gtest/gtest.h>
#include "audio/AudioManager.h"
#include "ecs/ECS.h"
#include "ecs/AudioSystems.h"
#include <SDL2/SDL.h>

using namespace Forge;

class AudioTest : public ::testing::Test {
protected:
    void SetUp() override {
        SDL_Init(SDL_INIT_AUDIO);
        AudioManager::Init();
    }

    void TearDown() override {
        ResourceManager::Clear();
        AudioManager::Shutdown();
        SDL_QuitSubSystem(SDL_INIT_AUDIO);
    }
};

TEST_F(AudioTest, Initialization) {
    // Should not crash
    EXPECT_TRUE(true);
}

TEST_F(AudioTest, AudioSourceComponent) {
    Registry registry;
    Entity e = registry.CreateEntity();

    AudioSource source;
    source.Volume = 0.5f;
    registry.AddComponent(e, source);

    EXPECT_TRUE(registry.HasComponent<AudioSource>(e));
    EXPECT_FLOAT_EQ(registry.GetComponent<AudioSource>(e).Volume, 0.5f);
}

TEST_F(AudioTest, AudioSystemInitializationFlag) {
    Registry registry;
    Entity e = registry.CreateEntity();

    AudioSource source;
    source.PlayOnAwake = true;
    registry.AddComponent(e, source);

    // Normally we'd load a real chunk, but without a file, it's null.
    // AudioSystem shouldn't crash on null clip.
    AudioSystem::Update(registry);

    EXPECT_FALSE(registry.GetComponent<AudioSource>(e).Initialized); // Fails to init without clip
}