#pragma once

#include "ecs/ECS.h"
#include <memory>

namespace Forge {

class ScriptableEntity;

/**
 * @brief Script component to attach custom logic to an entity.
 */
struct Script {
    std::shared_ptr<ScriptableEntity> Instance;

    std::shared_ptr<ScriptableEntity> (*InstantiateScript)();
    void (*DestroyScript)(Script*);

    template<typename T>
    void Bind() {
        InstantiateScript = []() { return std::static_pointer_cast<ScriptableEntity>(std::make_shared<T>()); };
        DestroyScript = [](Script* script) { script->Instance.reset(); };
    }
};

/**
 * @brief Base class for all script instances.
 */
class ScriptableEntity {
public:
    virtual ~ScriptableEntity() = default;

    template<typename T>
    T& GetComponent() {
        return m_Registry->GetComponent<T>(m_Entity);
    }

    template<typename T>
    bool HasComponent() {
        return m_Registry->HasComponent<T>(m_Entity);
    }

protected:
    virtual void OnCreate() {}
    virtual void OnDestroy() {}
    virtual void OnUpdate(float dt) {}

private:
    Entity m_Entity = NULL_ENTITY;
    std::shared_ptr<Registry> m_Registry;

    friend class ScriptSystem;
};

} // namespace Forge