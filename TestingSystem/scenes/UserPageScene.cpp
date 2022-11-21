#include <string>
#include <memory>
#include "scenes.h"
#include "PaginationScene.hpp"
#include "../entities/entities.h"
#include "../utils/Logger.h"


UserPageScene::UserPageScene(std::shared_ptr<User> user)
{
    this->user = user;
    this->view.assign({
        "Login користувача: " + this->user->getUserLogin() + "\n",
        "Email: " + this->user->getEmail() + "\n",
        "Телефон: " + this->user->getPhone() + "\n",
        "Введіть 'd', щоб видалити користвача\n",
        "Введіть 'e', щоб модифікувати користвача\n",
        "Введіть 'r', щоб переглянути резултати тестів користвача\n",
        "Введіть 'b', щоб повернутися у меню\n"
    });
}

UserPageScene::~UserPageScene()
{
    Logger::log("UserPageScene", "Destructor");
}

BaseScene * UserPageScene::handleInput(StateManager& stateManager, const std::string& input) noexcept
{
    if (input == "d")
    {
        if (this->user == stateManager.getCurrentUser())
        {
            this->setWarning();
            this->view.push_front("Неможливо видалити поточного користувача\n");
            return this;
        }
        stateManager.deleteUser(this->user);
        return new AdminMenuScene();
    }
    else if (input == "e")
    {
        return new ManageUserScene(this->user);
    }
    else if (input == "r")
    {
        auto testReports = stateManager.getTestReportsByUser(user->getUserLogin());
        std::list<std::shared_ptr<TestReport>> filteredTestReports;
        
        for (auto it = testReports.begin(); it != testReports.end(); ++it)
        {
            if ((*it)->isTestCompleted())
                filteredTestReports.push_front(*it);
        }

        if (filteredTestReports.empty())
            return new MessageScene("Наразі немає жодного завершеного тесту у даного користувача", new AdminMenuScene());
        else
            return new PaginationScene<TestResultScene, AdminMenuScene, TestReport>(filteredTestReports);
    }
    else if (input == "b")
    {
        return new AdminMenuScene("Користувач був видаленний");
    }

    this->setWarning();
    return this;
}
