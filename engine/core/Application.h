#pragma once

#include "core/Window.h"
#include "core/Layer.h"
#include "core/EventDispatcher.h"
#include "core/Timer.h"
#include "core/InputManager.h"
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

    virtual ~Application() {
        for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it) {
            (*it)->OnDetach();
        }
    }

    void PushLayer(std::unique_ptr<Layer> layer) {
        layer->OnAttach();
        m_LayerStack.push_back(std::move(layer));
    }

    // Overload for raw pointer backward compatibility
    void PushLayer(Layer* layer) {
        PushLayer(std::unique_ptr<Layer>(layer));
    }

    void Run() {
        m_IsRunning = true;
        Timer timer;

        while (m_IsRunning) {
            float dt = timer.Elapsed();
            timer.Reset();

            InputManager::Update();

            // Event polling
            SDL_Event e;
            while (SDL_PollEvent(&e)) {
                if (e.type == SDL_QUIT) {
                    m_IsRunning = false;
                }
                InputManager::HandleEvent(e);
            }

            for (auto& layer : m_LayerStack) {
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
    std::vector<std::unique_ptr<Layer>> m_LayerStack;
};

} // namespace Forge