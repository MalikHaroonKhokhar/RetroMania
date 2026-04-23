#include "core/Application.h"
#include "scene/Scene.h"
#include "core/InputManager.h"
#include "ecs/ScriptComponents.h"
#include "ecs/ScriptSystems.h"
#include "ecs/PhysicsComponents.h"
#include "ecs/PhysicsSystems.h"
#include "ecs/RenderComponents.h"
#include "ecs/RenderSystems.h"

using namespace Forge;

class PlayerController : public ScriptableEntity {
public:
    void OnCreate() override {
        FORGE_LOG_INFO("Player created!");
    }

    void OnUpdate(float dt) override {
        if (!HasComponent<Rigidbody>()) return;

        auto& rb = GetComponent<Rigidbody>();
        float speed = 200.0f;

        rb.Velocity.x = 0.0f;
        rb.Velocity.y = 0.0f;

        if (InputManager::IsKeyHeld(SDL_SCANCODE_W)) {
            rb.Velocity.y = -speed;
        }
        if (InputManager::IsKeyHeld(SDL_SCANCODE_S)) {
            rb.Velocity.y = speed;
        }
        if (InputManager::IsKeyHeld(SDL_SCANCODE_A)) {
            rb.Velocity.x = -speed;
        }
        if (InputManager::IsKeyHeld(SDL_SCANCODE_D)) {
            rb.Velocity.x = speed;
        }
    }
};

class GameLayer : public Layer {
public:
    GameLayer() : Layer("GameLayer") {}

    void OnAttach() override {
        m_Scene = std::make_shared<Scene>();
        auto reg = m_Scene->GetRegistry();

        // Setup Player
        Entity player = m_Scene->CreateEntity("Player");
        reg->AddComponent(player, Transform{100.0f, 100.0f, 0.0f, 1.0f, 1.0f});

        // We will just draw a basic rect instead of loading an image for demo simplicity
        // But we need a dummy texture to pass the RenderSystem check or we modify RenderSystem.
        // For this demo, let's assume we want to use the rendering capabilities without crashing
        // if no texture. In our `RenderSystem`, `!sprite.TexturePtr` makes it skip.
        // We can just rely on `Renderer::DrawRect` manually or we could add a simple 1x1 texture.

        Rigidbody rb;
        rb.GravityScale = 0.0f; // Top-down
        reg->AddComponent(player, rb);

        BoxCollider bc;
        bc.Size = {32.0f, 32.0f};
        reg->AddComponent(player, bc);

        Script script;
        script.Bind<PlayerController>();
        reg->AddComponent(player, script);

        // Setup Wall
        Entity wall = m_Scene->CreateEntity("Wall");
        reg->AddComponent(wall, Transform{300.0f, 100.0f, 0.0f, 1.0f, 1.0f});

        Rigidbody wallRb;
        wallRb.IsStatic = true;
        reg->AddComponent(wall, wallRb);

        BoxCollider wallBc;
        wallBc.Size = {32.0f, 100.0f};
        reg->AddComponent(wall, wallBc);
    }

    void OnUpdate(float dt) override {
        Renderer::SetClearColor(Color(50, 50, 50, 255));
        Renderer::BeginFrame();

        auto reg = m_Scene->GetRegistry();

        // Run systems
        ScriptSystem::Update(reg, dt);
        PhysicsSystem::Update(*reg, dt);

        // Custom debug rendering for the demo since we might not have assets
        auto view = reg->View<Transform, BoxCollider>();
        for (Entity e : view) {
            auto& t = reg->GetComponent<Transform>(e);
            auto& bc = reg->GetComponent<BoxCollider>(e);

            Color col = reg->GetComponent<Tag>(e).Name == "Player" ? Color(0, 255, 0) : Color(255, 0, 0);
            Renderer::DrawFilledRect({t.x + bc.Offset.x, t.y + bc.Offset.y}, bc.Size, col);
        }

        Renderer::EndFrame();
    }

private:
    std::shared_ptr<Scene> m_Scene;
};

int main(int argc, char* argv[]) {
    Log::Init();
    InputManager::Init();

    Application app(WindowProps("Forge2D Demo", 800, 600));
    Renderer::Init(app.GetWindow()); // assuming we expose GetWindow

    app.PushLayer(new GameLayer());

    app.Run();

    Renderer::Shutdown();
    InputManager::Shutdown();

    return 0;
}