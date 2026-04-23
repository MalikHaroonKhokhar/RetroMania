#pragma once

#include "ecs/ECS.h"
#include "ecs/RenderComponents.h"
#include "rendering/Renderer.h"
#include <algorithm>

namespace Forge {

class RenderSystem {
public:
    static void Update(Registry& registry) {
        auto view = registry.View<Transform, Sprite>();

        // Sort entities by z-index if we had one.
        // For now, we will just iterate and render.

        for (Entity e : view) {
            auto& transform = registry.GetComponent<Transform>(e);
            auto& sprite = registry.GetComponent<Sprite>(e);

            if (!sprite.TexturePtr) continue;

            Vec2 pos = { transform.x, transform.y };
            Vec2 size = {
                (float)(sprite.UseSourceRect ? sprite.SourceRect.w : sprite.TexturePtr->GetWidth()) * transform.scaleX,
                (float)(sprite.UseSourceRect ? sprite.SourceRect.h : sprite.TexturePtr->GetHeight()) * transform.scaleY
            };

            if (sprite.UseSourceRect) {
                SDL_Rect destRect = { (int)pos.x, (int)pos.y, (int)size.x, (int)size.y };
                Renderer::DrawTextureEx(sprite.TexturePtr, &sprite.SourceRect, &destRect, transform.rotation, nullptr, SDL_FLIP_NONE, sprite.Tint);
            } else {
                Renderer::DrawTexture(sprite.TexturePtr, pos, size, transform.rotation, sprite.Tint);
            }
        }
    }
};

class AnimationSystem {
public:
    static void Update(Registry& registry, float dt) {
        auto view = registry.View<Sprite, Animator>();

        for (Entity e : view) {
            auto& sprite = registry.GetComponent<Sprite>(e);
            auto& animator = registry.GetComponent<Animator>(e);

            if (!animator.IsPlaying || animator.FrameCount <= 1 || animator.FrameDuration <= 0.0f) continue;

            animator.CurrentTime += dt;
            while (animator.CurrentTime >= animator.FrameDuration && animator.IsPlaying) {
                animator.CurrentTime -= animator.FrameDuration;
                animator.CurrentFrame++;

                if (animator.CurrentFrame >= animator.FrameCount) {
                    if (animator.Loop) {
                        animator.CurrentFrame = 0;
                    } else {
                        animator.CurrentFrame = animator.FrameCount - 1;
                        animator.IsPlaying = false;
                    }
                }
            }

            // Update Sprite SourceRect based on current frame
            sprite.UseSourceRect = true;
            sprite.SourceRect.x = animator.CurrentFrame * animator.FrameWidth;
            sprite.SourceRect.y = 0; // Assuming single row for now, can be expanded
            sprite.SourceRect.w = animator.FrameWidth;
            sprite.SourceRect.h = animator.FrameHeight;
        }
    }
};

} // namespace Forge