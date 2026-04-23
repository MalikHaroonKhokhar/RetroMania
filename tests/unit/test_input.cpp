#include <gtest/gtest.h>
#include "core/InputManager.h"
#include <SDL2/SDL.h>

using namespace Forge;

class InputManagerTest : public ::testing::Test {
protected:
    void SetUp() override {
        InputManager::Init();
    }

    void TearDown() override {
        InputManager::Shutdown();
    }
};

TEST_F(InputManagerTest, KeyPressedHeldReleased) {
    SDL_Event e;
    e.type = SDL_KEYDOWN;
    e.key.keysym.scancode = SDL_SCANCODE_SPACE;

    // Simulate Key Press
    InputManager::Update();
    InputManager::HandleEvent(e);

    EXPECT_TRUE(InputManager::IsKeyPressed(SDL_SCANCODE_SPACE));
    EXPECT_TRUE(InputManager::IsKeyHeld(SDL_SCANCODE_SPACE));
    EXPECT_FALSE(InputManager::IsKeyReleased(SDL_SCANCODE_SPACE));

    // Simulate Next Frame (Key still held)
    InputManager::Update();
    EXPECT_FALSE(InputManager::IsKeyPressed(SDL_SCANCODE_SPACE));
    EXPECT_TRUE(InputManager::IsKeyHeld(SDL_SCANCODE_SPACE));
    EXPECT_FALSE(InputManager::IsKeyReleased(SDL_SCANCODE_SPACE));

    // Simulate Key Release
    e.type = SDL_KEYUP;
    InputManager::Update();
    InputManager::HandleEvent(e);

    EXPECT_FALSE(InputManager::IsKeyPressed(SDL_SCANCODE_SPACE));
    EXPECT_FALSE(InputManager::IsKeyHeld(SDL_SCANCODE_SPACE));
    EXPECT_TRUE(InputManager::IsKeyReleased(SDL_SCANCODE_SPACE));
}

TEST_F(InputManagerTest, MousePosition) {
    SDL_Event e;
    e.type = SDL_MOUSEMOTION;
    e.motion.x = 100;
    e.motion.y = 200;

    InputManager::Update();
    InputManager::HandleEvent(e);

    Vec2 pos = InputManager::GetMousePosition();
    EXPECT_FLOAT_EQ(pos.x, 100.0f);
    EXPECT_FLOAT_EQ(pos.y, 200.0f);
}