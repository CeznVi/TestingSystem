#include <string>
#include "scenes.h"
#include "../utils/StringUtils.h"
#include "../utils/Logger.h"


LoginScene::LoginScene()
{
    this->view.assign({ "Введіть логін:" });
}

LoginScene::~LoginScene()
{
    Logger::log("LoginScene", "Destructor");
}

BaseScene * LoginScene::handleInput(StateManager& stateManager, const std::string& input) noexcept
{
    if (this->userLogin.empty())
    {
        this->userLogin = input;
        this->view.assign({ "Введіть пароль:" });
        return this;
    }
    else if (this->hashedPassword.empty())
    {
        this->hashedPassword = StringUtils::hash(input);

        auto user = stateManager.getUser(this->userLogin);

        if ((user.get() == nullptr) || (user->getHashedPassword() != this->hashedPassword))
        {
            return new UnauthorizedMenuScene("Такого користувача не існую");
        }
        else
        {
            stateManager.setCurrentUser(user);
            if (stateManager.isCurrentUserAdmin())
                return new AdminMenuScene();
            else
                return new UserMenuScene();
        }
    }
    return new ErrorScene("Unreachable condition in LoginScene");
}