#include "core/Window.h"
#include "core/Log.h"

namespace Forge {

static int s_WindowCount = 0;

Window::Window(const WindowProps& props) {
    if (s_WindowCount == 0) {
        if (SDL_Init(0) < 0) {
            FORGE_LOG_ERROR("Could not initialize base SDL2: ", SDL_GetError());
        }
    }
    s_WindowCount++;

    Init(props);
}

Window::~Window() {
    Shutdown();
}

void Window::Init(const WindowProps& props) {
    m_Data.Title = props.Title;
    m_Data.Width = props.Width;
    m_Data.Height = props.Height;

    FORGE_LOG_INFO("Creating window ", props.Title, " (", props.Width, ", ", props.Height, ")");

    // Force dummy driver for headless CI if needed
    if (std::getenv("CI")) {
        SDL_setenv("SDL_VIDEODRIVER", "dummy", 1);
        SDL_setenv("SDL_AUDIODRIVER", "dummy", 1);
    }

    if (SDL_InitSubSystem(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        FORGE_LOG_ERROR("Could not initialize SDL2 SubSystems: ", SDL_GetError());
        return;
    }

    m_Window = SDL_CreateWindow(
        m_Data.Title.c_str(),
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        m_Data.Width,
        m_Data.Height,
        SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI
    );

    if (!m_Window) {
        FORGE_LOG_ERROR("Could not create SDL2 Window: ", SDL_GetError());
    }
}

void Window::Shutdown() {
    if (m_Window) {
        SDL_DestroyWindow(m_Window);
        m_Window = nullptr;
    }
    SDL_QuitSubSystem(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

    s_WindowCount--;
    if (s_WindowCount == 0) {
        SDL_Quit();
    }
}

void Window::Update() {
    // Basic polling is handled by InputManager or Application level usually,
    // but typically swapping buffers happens here for OpenGL.
    // Since we are moving towards an SDL Renderer, we'll keep this light.
}

} // namespace Forge