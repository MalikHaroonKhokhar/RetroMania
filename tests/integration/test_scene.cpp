#include <gtest/gtest.h>
#include "scene/Scene.h"
#include "scene/SceneSerializer.h"
#include <filesystem>

using namespace Forge;

TEST(SceneTest, EntityCreationAndDestruction) {
    auto scene = std::make_shared<Scene>();
    Entity e = scene->CreateEntity("TestEntity");

    auto reg = scene->GetRegistry();
    EXPECT_TRUE(reg->HasComponent<Tag>(e));
    EXPECT_EQ(reg->GetComponent<Tag>(e).Name, "TestEntity");

    scene->DestroyEntity(e);
    EXPECT_FALSE(reg->HasComponent<Tag>(e));
}

TEST(SceneTest, SerializationRoundTrip) {
    auto scene = std::make_shared<Scene>();
    Entity e = scene->CreateEntity("Player");

    Transform t{10.0f, 20.0f, 45.0f, 2.0f, 2.0f};
    scene->GetRegistry()->AddComponent(e, t);

    SceneSerializer serializer(scene);
    std::string filepath = "test_scene.json";
    EXPECT_TRUE(serializer.Serialize(filepath));

    // Create a new scene and deserialize into it
    auto newScene = std::make_shared<Scene>();
    SceneSerializer deserializer(newScene);
    EXPECT_TRUE(deserializer.Deserialize(filepath));

    auto reg = newScene->GetRegistry();
    auto view = reg->View<Tag>();
    EXPECT_EQ(view.size(), 1);

    Entity loadedEntity = view[0];
    EXPECT_EQ(reg->GetComponent<Tag>(loadedEntity).Name, "Player");

    EXPECT_TRUE(reg->HasComponent<Transform>(loadedEntity));
    auto& loadedT = reg->GetComponent<Transform>(loadedEntity);
    EXPECT_FLOAT_EQ(loadedT.x, 10.0f);
    EXPECT_FLOAT_EQ(loadedT.y, 20.0f);
    EXPECT_FLOAT_EQ(loadedT.rotation, 45.0f);

    // Cleanup
    std::filesystem::remove(filepath);
}

TEST(SceneTest, SerializationMalformedJson) {
    std::string filepath = "malformed.json";
    std::ofstream out(filepath);
    out << "{ \"Entities\": [ { \"Tag\": "; // Broken JSON
    out.close();

    auto scene = std::make_shared<Scene>();
    SceneSerializer deserializer(scene);
    EXPECT_FALSE(deserializer.Deserialize(filepath));

    std::filesystem::remove(filepath);
}