#include <string>
#include "scenes.h"
#include "../utils/Logger.h"


MessageScene::MessageScene(const std::string& message, BaseScene* nextScene)
{
    this->view.assign({ message, "\nДля продовження натисніть ENTER\n" });
    this->nextScene.reset(nextScene);
}

MessageScene::~MessageScene()
{
    Logger::log("MessageScene", "Destructor");
}

BaseScene * MessageScene::handleInput(StateManager& stateManager, const std::string& input) noexcept
{
    return this->nextScene.release();
}
