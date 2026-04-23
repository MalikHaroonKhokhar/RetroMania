#include "rendering/Renderer.h"
#include "core/Log.h"
#include <SDL_image.h>

namespace Forge {

void Renderer::Init(Window* window) {
    FORGE_LOG_INFO("Initializing SDL Renderer");

    s_Renderer = SDL_CreateRenderer(window->GetNativeWindow(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!s_Renderer) {
        FORGE_LOG_ERROR("Failed to create SDL Renderer: ", SDL_GetError());
    }

    // Initialize SDL_image (PNG support)
    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        FORGE_LOG_ERROR("SDL_image could not initialize! SDL_image Error: ", IMG_GetError());
    }
}

void Renderer::Shutdown() {
    if (s_Renderer) {
        SDL_DestroyRenderer(s_Renderer);
        s_Renderer = nullptr;
    }
    IMG_Quit();
}

void Renderer::BeginFrame() {
    SDL_SetRenderDrawColor(s_Renderer, s_ClearColor.r, s_ClearColor.g, s_ClearColor.b, s_ClearColor.a);
    SDL_RenderClear(s_Renderer);
}

void Renderer::EndFrame() {
    SDL_RenderPresent(s_Renderer);
}

void Renderer::SetClearColor(const Color& color) {
    s_ClearColor = color;
}

std::shared_ptr<Texture> Renderer::LoadTexture(const std::string& path, const std::string& name) {
    if (auto cached = ResourceManager::Get<Texture>(name)) {
        return cached;
    }

    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (!loadedSurface) {
        FORGE_LOG_ERROR("Unable to load image ", path, "! SDL_image Error: ", IMG_GetError());
        return nullptr;
    }

    SDL_Texture* newTexture = SDL_CreateTextureFromSurface(s_Renderer, loadedSurface);
    int width = loadedSurface->w;
    int height = loadedSurface->h;

    SDL_FreeSurface(loadedSurface);

    if (!newTexture) {
        FORGE_LOG_ERROR("Unable to create texture from ", path, "! SDL Error: ", SDL_GetError());
        return nullptr;
    }

    return ResourceManager::Load<Texture>(name, newTexture, width, height);
}

void Renderer::DrawRect(const Vec2& position, const Vec2& size, const Color& color) {
    SDL_Rect rect = { (int)position.x, (int)position.y, (int)size.x, (int)size.y };
    SDL_SetRenderDrawColor(s_Renderer, color.r, color.g, color.b, color.a);
    SDL_RenderDrawRect(s_Renderer, &rect);
}

void Renderer::DrawFilledRect(const Vec2& position, const Vec2& size, const Color& color) {
    SDL_Rect rect = { (int)position.x, (int)position.y, (int)size.x, (int)size.y };
    SDL_SetRenderDrawColor(s_Renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(s_Renderer, &rect);
}

void Renderer::DrawLine(const Vec2& start, const Vec2& end, const Color& color) {
    SDL_SetRenderDrawColor(s_Renderer, color.r, color.g, color.b, color.a);
    SDL_RenderDrawLine(s_Renderer, (int)start.x, (int)start.y, (int)end.x, (int)end.y);
}

void Renderer::DrawTexture(std::shared_ptr<Texture> texture, const Vec2& position, const Vec2& size, float rotation, const Color& tint) {
    if (!texture) return;

    SDL_Rect destRect = { (int)position.x, (int)position.y, (int)size.x, (int)size.y };
    SDL_SetTextureColorMod(texture->GetNativeTexture(), tint.r, tint.g, tint.b);
    SDL_SetTextureAlphaMod(texture->GetNativeTexture(), tint.a);

    SDL_RenderCopyEx(s_Renderer, texture->GetNativeTexture(), nullptr, &destRect, rotation, nullptr, SDL_FLIP_NONE);
}

void Renderer::DrawTextureEx(std::shared_ptr<Texture> texture, const SDL_Rect* srcRect, const SDL_Rect* destRect, float rotation, const SDL_Point* center, SDL_RendererFlip flip, const Color& tint) {
    if (!texture) return;

    SDL_SetTextureColorMod(texture->GetNativeTexture(), tint.r, tint.g, tint.b);
    SDL_SetTextureAlphaMod(texture->GetNativeTexture(), tint.a);

    SDL_RenderCopyEx(s_Renderer, texture->GetNativeTexture(), srcRect, destRect, rotation, center, flip);
}

} // namespace Forge