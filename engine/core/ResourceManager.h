#pragma once

#include <unordered_map>
#include <string>
#include <memory>
#include <stdexcept>
#include "core/Log.h"

namespace Forge {

/**
 * @brief Base class for resources.
 */
class Resource {
public:
    virtual ~Resource() = default;
};

/**
 * @brief Manages loading and caching of resources.
 */
class ResourceManager {
public:
    template<typename T, typename... Args>
    static std::shared_ptr<T> Load(const std::string& name, Args&&... args) {
        if (s_Resources.find(name) != s_Resources.end()) {
            auto ptr = std::dynamic_pointer_cast<T>(s_Resources[name]);
            if (ptr) {
                FORGE_LOG_INFO("Cache hit for resource: ", name);
                return ptr;
            } else {
                FORGE_LOG_ERROR("Resource type mismatch on load cache hit: ", name);
                return nullptr;
            }
        }

        FORGE_LOG_INFO("Loading resource: ", name);
        std::shared_ptr<T> resource = std::make_shared<T>(std::forward<Args>(args)...);
        s_Resources[name] = resource;
        return resource;
    }

    template<typename T>
    static std::shared_ptr<T> TryGet(const std::string& name) {
        if (s_Resources.find(name) == s_Resources.end()) {
            return nullptr;
        }
        return std::dynamic_pointer_cast<T>(s_Resources[name]);
    }

    template<typename T>
    static std::shared_ptr<T> Get(const std::string& name) {
        if (s_Resources.find(name) == s_Resources.end()) {
            return nullptr;
        }
        auto ptr = std::dynamic_pointer_cast<T>(s_Resources[name]);
        if (!ptr) {
            FORGE_LOG_ERROR("Resource type mismatch: ", name);
        }
        return ptr;
    }

    static void Unload(const std::string& name) {
        if (s_Resources.find(name) != s_Resources.end()) {
            FORGE_LOG_INFO("Unloading resource: ", name);
            s_Resources.erase(name);
        }
    }

    static void Clear() {
        FORGE_LOG_INFO("Clearing all resources.");
        s_Resources.clear();
    }

private:
    inline static std::unordered_map<std::string, std::shared_ptr<Resource>> s_Resources;
};

} // namespace Forge