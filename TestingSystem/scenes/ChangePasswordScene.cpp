#include <string>
#include "scenes.h"
#include "../utils/StringUtils.h"
#include "../utils/Logger.h"


ChangePasswordScene::ChangePasswordScene()
{
    this->view.assign({ "Введіть новий пароль:" });
}

ChangePasswordScene::~ChangePasswordScene()
{
    Logger::log("ChangePasswordScene", "Destructor");
}

BaseScene * ChangePasswordScene::handleInput(StateManager& stateManager, const std::string& input) noexcept
{
    auto user = stateManager.getCurrentUser();
    stateManager.modifyUser(user->getUserLogin(), StringUtils::hash(input), user->getEmail(), user->getPhone());

    return new AdminMenuScene("Пароль змінено");
}
