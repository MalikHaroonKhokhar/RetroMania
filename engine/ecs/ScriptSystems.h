#pragma once

#include "ecs/ECS.h"
#include "ecs/ScriptComponents.h"

namespace Forge {

class ScriptSystem {
public:
    static void Update(std::shared_ptr<Registry> registry, float dt) {
        auto view = registry->View<Script>();

        for (Entity e : view) {
            auto& script = registry->GetComponent<Script>(e);

            if (!script.Instance) {
                script.Instance = script.InstantiateScript();
                script.Instance->m_Entity = e;
                script.Instance->m_Registry = registry;
                script.Instance->OnCreate();
            }

            script.Instance->OnUpdate(dt);
        }
    }

    static void Cleanup(std::shared_ptr<Registry> registry) {
        auto view = registry->View<Script>();
        for (Entity e : view) {
            auto& script = registry->GetComponent<Script>(e);
            if (script.Instance) {
                script.Instance->OnDestroy();
                script.DestroyScript(&script);
            }
        }
    }
};

} // namespace Forge