#include <gtest/gtest.h>
#include "core/Application.h"
#include "rendering/Renderer.h"
#include "core/InputManager.h"
#include "core/Log.h"

using namespace Forge;

// A simple layer to test engine boot and run for a few frames
class E2ETestLayer : public Layer {
public:
    E2ETestLayer(Application* app, int maxFrames)
        : Layer("E2ETestLayer"), m_App(app), m_MaxFrames(maxFrames) {}

    void OnUpdate(float dt) override {
        m_FramesRan++;

        if (m_FramesRan >= m_MaxFrames) {
            m_App->Stop();
        }
    }

    int GetFramesRan() const { return m_FramesRan; }

private:
    Application* m_App;
    int m_MaxFrames;
    int m_FramesRan = 0;
};

TEST(E2ETest, EngineBootAndShutdown) {
    Log::Init();
    InputManager::Init();

    Application app(WindowProps("E2E Test", 800, 600));
    Renderer::Init(app.GetWindow());

    auto layer = new E2ETestLayer(&app, 10);
    app.PushLayer(layer);

    app.Run(); // This should run for 10 frames and then stop

    EXPECT_EQ(layer->GetFramesRan(), 10);

    Renderer::Shutdown();
    InputManager::Shutdown();
}