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
        // Clear scene first... (For simplicity, we recreate the registry if it was fully supported, but we will just rely on the assumption of a new scene for this scope)

        if (json.contains("Entities") && json["Entities"].is_array()) {
            for (auto& entityJson : json["Entities"]) {
                if (!entityJson.is_object()) continue;

                std::string name = "Entity";
                if (entityJson.contains("Tag") && entityJson["Tag"].is_object()) {
                    if (entityJson["Tag"].contains("Name") && entityJson["Tag"]["Name"].is_string()) {
                        name = entityJson["Tag"]["Name"].get<std::string>();
                    }
                }

                Entity e = m_Scene->CreateEntity(name);

                if (entityJson.contains("Transform") && entityJson["Transform"].is_object()) {
                    Transform t;
                    auto transformJson = entityJson["Transform"];
                    if (transformJson.contains("x") && transformJson["x"].is_number()) t.x = transformJson["x"].get<float>();
                    if (transformJson.contains("y") && transformJson["y"].is_number()) t.y = transformJson["y"].get<float>();
                    if (transformJson.contains("rotation") && transformJson["rotation"].is_number()) t.rotation = transformJson["rotation"].get<float>();
                    if (transformJson.contains("scaleX") && transformJson["scaleX"].is_number()) t.scaleX = transformJson["scaleX"].get<float>();
                    if (transformJson.contains("scaleY") && transformJson["scaleY"].is_number()) t.scaleY = transformJson["scaleY"].get<float>();

                    if (registry->HasComponent<Transform>(e)) {
                        registry->RemoveComponent<Transform>(e); // Remove the default one added by create or just overwrite it
                    }
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