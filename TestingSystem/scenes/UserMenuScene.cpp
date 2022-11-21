#include <string>
#include "scenes.h"
#include "PaginationScene.hpp"
#include "../utils/Logger.h"


UserMenuScene::UserMenuScene()
{
    this->warning = false;
    this->initView();
}

UserMenuScene::UserMenuScene(std::string message)
{
    this->warning = false;
    this->initView();
    this->view.push_front(message + "\n");
}

UserMenuScene::~UserMenuScene()
{
    Logger::log("UserMenuScene", "Destructor");
}

BaseScene * UserMenuScene::handleInput(StateManager& stateManager, const std::string& input) noexcept
{
    if (input == "1")
    {
        auto subjects = stateManager.getSubjects();
        if (subjects.empty())
            return new MessageScene("Наразі немає жодного доступного тесту", new UserMenuScene());
        else
            return new UserSubjectListScene(subjects);
    }
    else if (input == "2")
    {
        auto testReports = stateManager.getTestReportsByUser(stateManager.getCurrentUser()->getUserLogin());
        std::list<std::shared_ptr<TestReport>> filteredTestReports;

        for (auto it = testReports.begin(); it != testReports.end(); ++it)
        {
            if ((*it)->isTestCompleted())
                filteredTestReports.push_front(*(it));
        }

        if (filteredTestReports.size() > 0)
            return new PaginationScene<TestResultScene, UserMenuScene, TestReport>(filteredTestReports);
        else
            return new MessageScene("Наразі немає жодного результатів тестування", new UserMenuScene());
    }
    else if (input == "3")
    {
        auto testReports = stateManager.getTestReportsByUser(stateManager.getCurrentUser()->getUserLogin());
        std::list<std::shared_ptr<TestReport>> filteredTestReports;

        for (auto it = testReports.begin(); it != testReports.end(); ++it)
        {
            if (!(*it)->isTestCompleted())
                filteredTestReports.push_front(*(it));
        }

        if (filteredTestReports.size() > 0)
            return new UserUnfinishedTestListScene(filteredTestReports);
        else
            return new MessageScene("Наразі ви не проходите жодного тесту", new UserMenuScene());
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

void UserMenuScene::initView()
{
    this->view.assign({
        "Ви знаходитесь у головному меню користувача. Оберіть дію: \n",
        "1) Пройти тест\n",
        "2) Переглянути результати пройдених тестів\n",
        "3) Повернутися до проходження тесту\n",
        "0) Вийти з облікового запису\n",
    });
}
