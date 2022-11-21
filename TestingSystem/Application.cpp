#include <memory>
#include <string>
#include <Windows.h>
#include "StateManager.h"
#include "UIController.h"
#include "scenes/scenes.h"
#include "utils/Logger.h"


class Application
{
public:
    Application();
    ~Application();
    int execute();

private:
    StateManager stateManager;
    UIController uiController;
};

Application::Application()
{
    Logger::log("Application", "Constructor");
}

Application::~Application()
{
    Logger::log("Application", "Destructor");
}

int Application::execute()
{
    std::unique_ptr<BaseScene> scene(new InitScene());
    while (scene)
    {
        uiController.renderScene(scene);
        auto input = uiController.getInput();
        auto newScene = scene->handleInput(this->stateManager, input);
        if (scene.get() != newScene)
        {
            scene.reset(newScene);
        }
    }

    return 0;
}


int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    Application app;
    auto exitCode = app.execute();

    return exitCode;
}