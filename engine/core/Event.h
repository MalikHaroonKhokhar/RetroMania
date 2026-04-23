#pragma once

namespace Forge {

/**
 * @brief Base event class.
 */
class Event {
public:
    virtual ~Event() = default;

    bool Handled = false;
};

} // namespace Forge