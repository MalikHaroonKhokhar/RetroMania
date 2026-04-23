#pragma once

#include "ecs/ECS.h"
#include <memory>

namespace Forge {

/**
 * @brief Represents a single game scene containing an ECS registry.
 */
class Scene {
public:
    Scene() {
        m_Registry = std::make_shared<Registry>();
    }

    ~Scene() = default;

    /**
     * @brief Creates an entity in the scene with a default tag.
     */
    Entity CreateEntity(const std::string& name = "Entity") {
        Entity entity = m_Registry->CreateEntity();
        m_Registry->AddComponent(entity, Tag{name});
        return entity;
    }

    /**
     * @brief Destroys an entity in the scene.
     */
    void DestroyEntity(Entity entity) {
        m_Registry->DestroyEntity(entity);
    }

    std::shared_ptr<Registry> GetRegistry() { return m_Registry; }

private:
    std::shared_ptr<Registry> m_Registry;
};

} // namespace Forge