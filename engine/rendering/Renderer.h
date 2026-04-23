#pragma once

#include <SDL2/SDL.h>
#include <string>
#include "core/Window.h"
#include "math/Vec2.h"
#include "core/ResourceManager.h"

namespace Forge {

/**
 * @brief SDL2 Texture wrapper resource.
 */
class Texture : public Resource {
public:
    Texture(SDL_Texture* texture, int width, int height)
        : m_Texture(texture), m_Width(width), m_Height(height) {}

    ~Texture() {
        if (m_Texture) {
            SDL_DestroyTexture(m_Texture);
        }
    }

    SDL_Texture* GetNativeTexture() const { return m_Texture; }
    int GetWidth() const { return m_Width; }
    int GetHeight() const { return m_Height; }

private:
    SDL_Texture* m_Texture = nullptr;
    int m_Width = 0;
    int m_Height = 0;
};

/**
 * @brief Represents a simple color structure.
 */
struct Color {
    uint8_t r, g, b, a;
    Color(uint8_t r = 255, uint8_t g = 255, uint8_t b = 255, uint8_t a = 255) : r(r), g(g), b(b), a(a) {}
};

/**
 * @brief 2D Renderer abstraction.
 */
class Renderer {
public:
    static void Init(Window* window);
    static void Shutdown();

    static void BeginFrame();
    static void EndFrame();
    static void SetClearColor(const Color& color);

    static std::shared_ptr<Texture> LoadTexture(const std::string& path, const std::string& name);

    // Drawing
    static void DrawRect(const Vec2& position, const Vec2& size, const Color& color);
    static void DrawFilledRect(const Vec2& position, const Vec2& size, const Color& color);
    static void DrawLine(const Vec2& start, const Vec2& end, const Color& color);

    static void DrawTexture(std::shared_ptr<Texture> texture, const Vec2& position, const Vec2& size, float rotation = 0.0f, const Color& tint = Color());
    static void DrawTextureEx(std::shared_ptr<Texture> texture, const SDL_Rect* srcRect, const SDL_Rect* destRect, float rotation, const SDL_Point* center, SDL_RendererFlip flip, const Color& tint = Color());

    static SDL_Renderer* GetNativeRenderer() { return s_Renderer; }

private:
    inline static SDL_Renderer* s_Renderer = nullptr;
    inline static Color s_ClearColor = {0, 0, 0, 255};
};

} // namespace Forge