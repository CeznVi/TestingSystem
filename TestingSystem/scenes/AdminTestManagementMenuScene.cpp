#include <string>
#include "scenes.h"
#include "PaginationScene.hpp"
#include "../utils/Logger.h"


AdminTestManagementMenuScene::AdminTestManagementMenuScene()
{
    this->warning = false;
    this->initView();
}

AdminTestManagementMenuScene::AdminTestManagementMenuScene(std::string message)
{
    this->warning = false;
    this->initView();
    this->view.push_front(message + "\n");
}

AdminTestManagementMenuScene::~AdminTestManagementMenuScene()
{
    Logger::log("AdminTestManagementMenuScene", "Destructor");
}

BaseScene * AdminTestManagementMenuScene::handleInput(StateManager& stateManager, const std::string& input) noexcept
{
    if (input == "1")
    {
        auto subjects = stateManager.getSubjects();
        if (subjects.empty())
            return new MessageScene("Наразі немає жодної категорії", new AdminTestManagementMenuScene());
        else
            return new PaginationScene<ExportScene, AdminTestManagementMenuScene, Subject>(subjects);
    }
    else if (input == "2")
    {
        auto tests = stateManager.getTests();
        if (tests.empty())
            return new MessageScene("Наразі немає жодного тесту", new AdminTestManagementMenuScene());
        else
            return new PaginationScene<ExportScene, AdminTestManagementMenuScene, Test>(tests);
    }
    else if (input == "3")
    {
        return new ImportScene();
    }
    else if (input == "4")
    {
        return new NewSubjectScene();
    }
    else if (input == "5")
    {
        return new NewTestScene();
    }
    else if (input == "0")
    {
        return new AdminMenuScene();
    }
    else if (!this->warning)
    {
        this->initView();
        this->setWarning();
    }
    return this;
}

void AdminTestManagementMenuScene::initView()
{
    this->view.assign({
        "Ви знаходитесь у головному меню керування тестами. Оберіть дію: \n",
        "1) Переглянути категорії тестів (для подальшого експорту)\n",
        "2) Переглянути список тестів (для подальшого експорту)\n",
        "3) Імпорт із файлу\n",
        "4) Створити нову категорію для тестів\n",
        "5) Створити новий тест\n",
        "0) Вийти до головного меню Адміністратора\n",
    });
}
