#include <string>
#include <memory>
#include <list>
#include <set>
#include "scenes.h"
#include "PaginationScene.hpp"
#include "../utils/Logger.h"


BaseScene* UserSubjectListScene::processEnitity(std::shared_ptr<Subject> subject, StateManager& stateManager)
{
    auto tests = stateManager.getTests();
    auto userTestReports = stateManager.getTestReportsByUser(stateManager.getCurrentUser()->getUserLogin());
    std::set<unsigned int> userTestIds; 
    std::list<std::shared_ptr<Test>> filteredTests;

    for (auto it = userTestReports.begin(); it != userTestReports.end(); ++it)
    {
        userTestIds.insert((*it)->getTestId());
    }

    for (auto it = tests.begin(); it != tests.end(); ++it)
    {
        if ((*it)->getSubject() == subject->getName() && userTestIds.find((*it)->getTestId()) == userTestIds.end())
            filteredTests.push_front(*(it));
    }

    if (filteredTests.size() > 0)
        return new PaginationScene<UserTestQuestionScene, UserMenuScene, Test>(filteredTests);
    else
        return new UserMenuScene("За даною категорією немає доступних тестів");
}
