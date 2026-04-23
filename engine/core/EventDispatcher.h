#pragma once

#include "core/Event.h"
#include <functional>
#include <vector>
#include <unordered_map>
#include <typeindex>
#include <memory>

namespace Forge {

/**
 * @brief Dispatcher for managing and dispatching events.
 */
class EventDispatcher {
public:
    template<typename T>
    using EventCallback = std::function<void(T&)>;

    /**
     * @brief Subscribe to an event type.
     */
    template<typename T>
    void Subscribe(EventCallback<T> callback) {
        m_Callbacks[typeid(T)].push_back([callback](Event& event) {
#ifndef NDEBUG
            if (auto* casted = dynamic_cast<T*>(&event)) {
                callback(*casted);
            }
#else
            callback(static_cast<T&>(event));
#endif
        });
    }

    /**
     * @brief Dispatch an event to all subscribers.
     */
    template<typename T>
    void Dispatch(T& event) {
        auto it = m_Callbacks.find(typeid(std::remove_reference_t<decltype(event)>));
        if (it != m_Callbacks.end()) {
            for (auto& callback : it->second) {
                if (event.Handled) break;
                callback(event);
            }
        }
    }

private:
    using GenericCallback = std::function<void(Event&)>;
    std::unordered_map<std::type_index, std::vector<GenericCallback>> m_Callbacks;
};

} // namespace Forge