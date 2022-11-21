#include <string>
#include <memory>
#include <list>
#include "scenes.h"
#include "PaginationScene.hpp"
#include "../entities/entities.h"
#include "../utils/Logger.h"


ViewResultMenuScene::ViewResultMenuScene()
{
    this->warning = false;
    this->initView();
}

ViewResultMenuScene::ViewResultMenuScene(std::string message)
{
    this->warning = false;
    this->initView();
    this->view.push_front(message + "\n");
}

ViewResultMenuScene::~ViewResultMenuScene()
{
    Logger::log("ViewResultMenuScene", "Destructor");
}

BaseScene * ViewResultMenuScene::handleInput(StateManager& stateManager, const std::string& input) noexcept
{
    if (input == "1")
    {
        auto subjects = stateManager.getSubjects();
        if (subjects.empty())
            return new MessageScene("Наразі немає жодної категорії", new ViewResultMenuScene());
        else
            return new SubjectsListForResultsScene(subjects);
    }
    else if (input == "2")
    {
        auto tests = stateManager.getTests();
        if (tests.empty())
            return new MessageScene("Наразі немає жодного тесту", new ViewResultMenuScene());
        else
            return new TestsListForResultsScene(tests);
    }
    else if (input == "3")
    {
        auto users = stateManager.getUsers();
        std::list<std::shared_ptr<User>> filteredUsers;
        for (auto it = users.begin(); it != users.end(); ++it)
        {
            if (!(*it)->getIsAdmin())
                filteredUsers.push_front(*it);
        }

        if (filteredUsers.empty())
            return new MessageScene("Наразі немає жодного користувача", new ViewResultMenuScene());
        else
            return new UsersListForResultsScene(filteredUsers);
    }
    else if (input == "4")
    {
        auto testReports= stateManager.getTestReports();
        std::list<std::shared_ptr<TestReport>> filteredTestReports;

        for (auto it = testReports.begin(); it != testReports.end(); ++it)
        {
            if ((*it)->isTestCompleted())
                filteredTestReports.push_front(*it);
        }

        if (filteredTestReports.empty())
            return new MessageScene("Наразі немає жодного завершеного тесту", new ViewResultMenuScene());
        else
            return new PaginationScene<TestResultScene, ViewResultMenuScene, TestReport>(filteredTestReports);
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

void ViewResultMenuScene::initView()
{
    this->view.assign({
        "Ви знаходитесь у меню перегляду результатів. Оберіть дію: \n",
        "1) Переглянути категорії тестів (для подальшого групування резултатів за категорією)\n",
        "2) Переглянути список тестів (для подальшого групування резултатів за тестами)\n",
        "3) Переглянути список користувачів (для подальшого групування резултатів за користувачами)\n",
        "4) Переглянути список результатів без групувань\n",
        "0) Вийти до головного меню Адміністратора\n",
    });
}
