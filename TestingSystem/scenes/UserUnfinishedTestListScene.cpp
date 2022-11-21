#include <string>
#include <memory>
#include <list>
#include <set>
#include "scenes.h"
#include "PaginationScene.hpp"
#include "../utils/Logger.h"


BaseScene* UserUnfinishedTestListScene::processEnitity(std::shared_ptr<TestReport> testReport, StateManager& stateManager)
{
    return new UserTestQuestionScene(stateManager.getTestById(testReport->getTestId()), testReport);
}
