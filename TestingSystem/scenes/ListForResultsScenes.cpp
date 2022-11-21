#include <string>
#include <memory>
#include <list>
#include <map>
#include "scenes.h"
#include "PaginationScene.hpp"
#include "../utils/Logger.h"


BaseScene* UsersListForResultsScene::processEnitity(std::shared_ptr<User> user, StateManager& stateManager)
{
    auto testReports = stateManager.getTestReportsByUser(user->getUserLogin());
    std::list<std::shared_ptr<TestReport>> filteredTestReports;
    
    for (auto it = testReports.begin(); it != testReports.end(); ++it)
    {
        if ((*it)->isTestCompleted())
            filteredTestReports.push_front(*it);
    }

    if (filteredTestReports.empty())
        return new MessageScene("Наразі немає жодного завершеного тесту у даного користувача", new ViewResultMenuScene());
    else
        return new PaginationScene<TestResultScene, ViewResultMenuScene, TestReport>(filteredTestReports);
}

BaseScene* TestsListForResultsScene::processEnitity(std::shared_ptr<Test> test, StateManager& stateManager)
{
    auto testReports = stateManager.getTestReports();
    std::list<std::shared_ptr<TestReport>> filteredTestReports;
    
    for (auto it = testReports.begin(); it != testReports.end(); ++it)
    {
        if ((*it)->isTestCompleted() && (*it)->getTestId() == test->getTestId())
            filteredTestReports.push_front(*it);
    }

    if (filteredTestReports.empty())
        return new MessageScene("Наразі немає жодного завершеного тесту", new ViewResultMenuScene());
    else
        return new PaginationScene<TestResultScene, ViewResultMenuScene, TestReport>(filteredTestReports);
}

BaseScene* SubjectsListForResultsScene::processEnitity(std::shared_ptr<Subject> subject, StateManager& stateManager)
{
    auto testReports = stateManager.getTestReports();
    auto tests = stateManager.getTests();
    std::map<unsigned int, std::string> testIdToSubject;
    std::list<std::shared_ptr<TestReport>> filteredTestReports;
    
    for (auto it = tests.begin(); it != tests.end(); ++it)
    {
        testIdToSubject[(*it)->getTestId()] = (*it)->getSubject();
    }

    for (auto it = testReports.begin(); it != testReports.end(); ++it)
    {
        if ((*it)->isTestCompleted() && 
            testIdToSubject[(*it)->getTestId()] == subject->getName())
            filteredTestReports.push_front(*it);
    }

    if (filteredTestReports.empty())
        return new MessageScene("Наразі немає жодного завершеного тесту за даною категорією", new ViewResultMenuScene());
    else
        return new PaginationScene<TestResultScene, ViewResultMenuScene, TestReport>(filteredTestReports);
}
