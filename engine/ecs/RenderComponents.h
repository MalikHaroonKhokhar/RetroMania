#pragma once

#include "rendering/Renderer.h"
#include <memory>

namespace Forge {

/**
 * @brief Sprite component for rendering 2D textures.
 */
struct Sprite {
    std::shared_ptr<Texture> TexturePtr;
    Color Tint = Color(255, 255, 255, 255);

    // For sprite sheets
    bool UseSourceRect = false;
    SDL_Rect SourceRect = {0, 0, 0, 0};

    Sprite() = default;
    Sprite(std::shared_ptr<Texture> texture) : TexturePtr(texture) {}
    Sprite(std::shared_ptr<Texture> texture, const Color& tint) : TexturePtr(texture), Tint(tint) {}
};

/**
 * @brief Animator component for sprite sheet animations.
 */
struct Animator {
    float CurrentTime = 0.0f;
    float FrameDuration = 0.1f;
    int CurrentFrame = 0;
    int FrameCount = 1;

    int FrameWidth = 0;
    int FrameHeight = 0;

    bool IsPlaying = true;
    bool Loop = true;
};

/**
 * @brief Camera component for 2D orthographic projection.
 */
struct Camera {
    float Zoom = 1.0f;
    // Additional camera properties can be added here
};

} // namespace Forge