#pragma once

#include "core/Event.h"
#include <string>

namespace Forge {

/**
 * @brief Represents a game state or layer in the application.
 */
class Layer {
public:
    Layer(const std::string& name = "Layer") : m_DebugName(name) {}
    virtual ~Layer() = default;

    virtual void OnAttach() {}
    virtual void OnDetach() {}
    virtual void OnUpdate(float dt) {}
    virtual void OnEvent(Event& event) {}

    const std::string& GetName() const { return m_DebugName; }

protected:
    std::string m_DebugName;
};

} // namespace Forge