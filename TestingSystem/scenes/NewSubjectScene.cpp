#include <string>
#include "scenes.h"
#include "../utils/Logger.h"


NewSubjectScene::NewSubjectScene()
{
    this->view.assign({ "Введіть назву нової категорії:" });
}

NewSubjectScene::~NewSubjectScene()
{
    Logger::log("NewSubjectScene", "Destructor");
}

BaseScene * NewSubjectScene::handleInput(StateManager& stateManager, const std::string& input) noexcept
{
    stateManager.addSubject(input);
    return new AdminTestManagementMenuScene("Нову категорію створенно");
}
