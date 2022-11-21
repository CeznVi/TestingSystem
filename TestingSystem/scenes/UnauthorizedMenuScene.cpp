#include <string>
#include "scenes.h"
#include "../utils/Logger.h"


UnauthorizedMenuScene::UnauthorizedMenuScene()
{
    this->warning = false;
    this->initView();
}

UnauthorizedMenuScene::UnauthorizedMenuScene(std::string message)
{
    this->warning = false;
    this->initView();
    this->view.push_front(message + "\n");
}

UnauthorizedMenuScene::~UnauthorizedMenuScene()
{
    Logger::log("UnauthorizedMenuScene", "Destructor");
}

BaseScene * UnauthorizedMenuScene::handleInput(StateManager& stateManager, const std::string& input) noexcept
{
    if (input == "1")
    {
        return new LoginScene();
    }
    else if (input == "2")
    {
        return new ManageUserScene();
    } 
    else if (input == "0")
    {
        return nullptr;
    }
    else if (!this->warning)
    {
        this->initView();
        this->setWarning();
    }
    return this;
}

void UnauthorizedMenuScene::initView()
{
    this->view.assign({
        "Оберіть дію: \n",
        "1) Логін\n"
        "2) Cтворити нового користувача\n",
        "0) Вихід\n",
    });
}
