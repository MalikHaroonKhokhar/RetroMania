#pragma once

#include "math/Vec2.h"
#include <unordered_map>
#include <SDL2/SDL.h>

namespace Forge {

/**
 * @brief Manages keyboard and mouse input states.
 */
class InputManager {
public:
    static void Init() {
        s_Instance = new InputManager();
    }

    static void Shutdown() {
        delete s_Instance;
        s_Instance = nullptr;
    }

    static void Update() {
        // Copy current state to previous state
        s_Instance->m_PreviousKeyStates = s_Instance->m_CurrentKeyStates;
        s_Instance->m_PreviousMouseStates = s_Instance->m_CurrentMouseStates;
    }

    /**
     * @brief Handle an SDL event (called by Window/Application).
     */
    static void HandleEvent(const SDL_Event& e) {
        if (!s_Instance) return;

        if (e.type == SDL_KEYDOWN) {
            s_Instance->m_CurrentKeyStates[e.key.keysym.scancode] = true;
        } else if (e.type == SDL_KEYUP) {
            s_Instance->m_CurrentKeyStates[e.key.keysym.scancode] = false;
        } else if (e.type == SDL_MOUSEBUTTONDOWN) {
            s_Instance->m_CurrentMouseStates[e.button.button] = true;
        } else if (e.type == SDL_MOUSEBUTTONUP) {
            s_Instance->m_CurrentMouseStates[e.button.button] = false;
        } else if (e.type == SDL_MOUSEMOTION) {
            s_Instance->m_MousePosition.x = (float)e.motion.x;
            s_Instance->m_MousePosition.y = (float)e.motion.y;
        }
    }

    // Keyboard
    static bool IsKeyPressed(int scancode) {
        if (!s_Instance) return false;
        return s_Instance->m_CurrentKeyStates[scancode] && !s_Instance->m_PreviousKeyStates[scancode];
    }

    static bool IsKeyHeld(int scancode) {
        if (!s_Instance) return false;
        return s_Instance->m_CurrentKeyStates[scancode];
    }

    static bool IsKeyReleased(int scancode) {
        if (!s_Instance) return false;
        return !s_Instance->m_CurrentKeyStates[scancode] && s_Instance->m_PreviousKeyStates[scancode];
    }

    // Mouse
    static bool IsMouseButtonPressed(int button) {
        if (!s_Instance) return false;
        return s_Instance->m_CurrentMouseStates[button] && !s_Instance->m_PreviousMouseStates[button];
    }

    static bool IsMouseButtonHeld(int button) {
        if (!s_Instance) return false;
        return s_Instance->m_CurrentMouseStates[button];
    }

    static bool IsMouseButtonReleased(int button) {
        if (!s_Instance) return false;
        return !s_Instance->m_CurrentMouseStates[button] && s_Instance->m_PreviousMouseStates[button];
    }

    static Vec2 GetMousePosition() {
        if (!s_Instance) return {0.0f, 0.0f};
        return s_Instance->m_MousePosition;
    }

private:
    InputManager() = default;
    ~InputManager() = default;

    inline static InputManager* s_Instance = nullptr;

    std::unordered_map<int, bool> m_CurrentKeyStates;
    std::unordered_map<int, bool> m_PreviousKeyStates;

    std::unordered_map<int, bool> m_CurrentMouseStates;
    std::unordered_map<int, bool> m_PreviousMouseStates;

    Vec2 m_MousePosition;
};

} // namespace Forge