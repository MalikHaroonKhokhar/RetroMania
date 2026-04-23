#pragma once

#include "core/Window.h"
#include "core/Layer.h"
#include "core/EventDispatcher.h"
#include "core/Timer.h"
#include <vector>
#include <memory>

namespace Forge {

/**
 * @brief Core Application loop.
 */
class Application {
public:
    Application(const WindowProps& props = WindowProps()) {
        m_Window = std::make_unique<Window>(props);
    }

    virtual ~Application() = default;

    void PushLayer(Layer* layer) {
        m_LayerStack.push_back(layer);
        layer->OnAttach();
    }

    void Run() {
        m_IsRunning = true;
        Timer timer;

        while (m_IsRunning) {
            float dt = timer.Elapsed();
            timer.Reset();

            // Simulate fixed timestep for physics later
            // Event polling
            SDL_Event e;
            while (SDL_PollEvent(&e)) {
                if (e.type == SDL_QUIT) {
                    m_IsRunning = false;
                }
                // Forward to input manager etc.
            }

            for (Layer* layer : m_LayerStack) {
                layer->OnUpdate(dt);
            }

            m_Window->Update();
        }
    }

    void Stop() { m_IsRunning = false; }

    Window* GetWindow() { return m_Window.get(); }

private:
    std::unique_ptr<Window> m_Window;
    bool m_IsRunning = false;
    std::vector<Layer*> m_LayerStack;
};

} // namespace Forge