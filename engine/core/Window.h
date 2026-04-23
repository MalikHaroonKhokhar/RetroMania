#pragma once

#include <string>
#include <SDL2/SDL.h>

namespace Forge {

/**
 * @brief Window configuration properties.
 */
struct WindowProps {
    std::string Title;
    uint32_t Width;
    uint32_t Height;

    WindowProps(const std::string& title = "Forge2D Engine",
                uint32_t width = 1280,
                uint32_t height = 720)
        : Title(title), Width(width), Height(height) {}
};

/**
 * @brief SDL2 Window abstraction.
 */
class Window {
public:
    Window(const WindowProps& props);
    ~Window();

    void Update();

    uint32_t GetWidth() const { return m_Data.Width; }
    uint32_t GetHeight() const { return m_Data.Height; }

    SDL_Window* GetNativeWindow() const { return m_Window; }

private:
    void Init(const WindowProps& props);
    void Shutdown();

    SDL_Window* m_Window = nullptr;

    struct WindowData {
        std::string Title;
        uint32_t Width, Height;
    };

    WindowData m_Data;
};

} // namespace Forge