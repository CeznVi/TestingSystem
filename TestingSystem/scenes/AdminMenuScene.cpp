#include <string>
#include "scenes.h"
#include "PaginationScene.hpp"
#include "../utils/Logger.h"


AdminMenuScene::AdminMenuScene()
{
    this->warning = false;
    this->initView();
}

AdminMenuScene::AdminMenuScene(std::string message)
{
    this->warning = false;
    this->initView();
    this->view.push_front(message + "\n");
}

AdminMenuScene::~AdminMenuScene()
{
    Logger::log("AdminMenuScene", "Destructor");
}

BaseScene * AdminMenuScene::handleInput(StateManager& stateManager, const std::string& input) noexcept
{
    if (input == "1")
    {
        return new PaginationScene<UserPageScene, AdminMenuScene, User>(stateManager.getUsers());
    }
    else if (input == "2")
    {
        return new ManageUserScene(false);
    }
    else if (input == "3")
    {
        return new ViewResultMenuScene();
    }
    else if (input == "4")
    {
        return new AdminTestManagementMenuScene();
    }
    else if (input == "5")
    {
        return new ChangePasswordScene();
    }
    else if (input == "0")
    {
        return new UnauthorizedMenuScene();
    }
    else if (!this->warning)
    {
        this->initView();
        this->setWarning();
    }
    return this;
}

void AdminMenuScene::initView()
{
    this->view.assign({
        "Ви знаходитесь у головному меню Адміністратора. Оберіть дію: \n",
        "1) Переглянути користувачів\n",
        "2) Створити користувача\n"
        "3) Переглянути результати\n",
        "4) Перейти до меню керування тестами\n",
        "5) Змінити пароль даного облікового запису\n",
        "0) Вийти з облікового запису\n",
    });
}
