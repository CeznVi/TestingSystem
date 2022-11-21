#include <string>
#include "scenes.h"
#include "../utils/Logger.h"


ErrorScene::ErrorScene(const std::string& errorMessage)
{
    this->view.assign({
        "Сталося помилка. Програма завершує работу.\n",
        errorMessage,
        "\nДля завершення натисніть ENTER\n"
    });
}

ErrorScene::~ErrorScene()
{
    Logger::log("ErrorScene", "Destructor");
}

BaseScene * ErrorScene::handleInput(StateManager& stateManager, const std::string& input) noexcept
{
    return nullptr;
}
