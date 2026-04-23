#pragma once

#include "scene/Scene.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include "core/Log.h"
#include "ecs/PhysicsComponents.h"
#include "ecs/RenderComponents.h"

namespace Forge {

/**
 * @brief Serializes and deserializes a scene to/from JSON.
 */
class SceneSerializer {
public:
    SceneSerializer(std::shared_ptr<Scene> scene) : m_Scene(scene) {}

    bool Serialize(const std::string& filepath) {
        nlohmann::json json;
        auto registry = m_Scene->GetRegistry();

        // This is a simplified serialization.
        // A complete engine would map components generically.
        auto view = registry->View<Tag>();
        for (Entity e : view) {
            nlohmann::json entityJson;
            entityJson["Entity"] = e;

            if (registry->HasComponent<Tag>(e)) {
                entityJson["Tag"]["Name"] = registry->GetComponent<Tag>(e).Name;
            }

            if (registry->HasComponent<Transform>(e)) {
                auto& t = registry->GetComponent<Transform>(e);
                entityJson["Transform"] = {
                    {"x", t.x}, {"y", t.y},
                    {"rotation", t.rotation},
                    {"scaleX", t.scaleX}, {"scaleY", t.scaleY}
                };
            }

            // Could serialize more components here...

            json["Entities"].push_back(entityJson);
        }

        std::ofstream fout(filepath);
        if (fout.is_open()) {
            fout << json.dump(4);
            fout.close();
            return true;
        }

        FORGE_LOG_ERROR("Could not open file for writing: ", filepath);
        return false;
    }

    bool Deserialize(const std::string& filepath) {
        std::ifstream stream(filepath);
        if (!stream.is_open()) {
            FORGE_LOG_ERROR("Could not open file for reading: ", filepath);
            return false;
        }

        nlohmann::json json;
        try {
            stream >> json;
        } catch (nlohmann::json::parse_error& e) {
            FORGE_LOG_ERROR("JSON Parse Error: ", e.what());
            return false;
        }

        auto registry = m_Scene->GetRegistry();
        // Clear scene first... (For simplicity, we assume new scene)

        if (json.contains("Entities")) {
            for (auto& entityJson : json["Entities"]) {
                std::string name = entityJson["Tag"]["Name"].get<std::string>();
                Entity e = m_Scene->CreateEntity(name);

                if (entityJson.contains("Transform")) {
                    Transform t;
                    t.x = entityJson["Transform"]["x"].get<float>();
                    t.y = entityJson["Transform"]["y"].get<float>();
                    t.rotation = entityJson["Transform"]["rotation"].get<float>();
                    t.scaleX = entityJson["Transform"]["scaleX"].get<float>();
                    t.scaleY = entityJson["Transform"]["scaleY"].get<float>();
                    registry->AddComponent(e, t);
                }
            }
        }

        return true;
    }

private:
    std::shared_ptr<Scene> m_Scene;
};

} // namespace Forge