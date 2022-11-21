#include <string>
#include <memory>
#include "scenes.h"
#include "../entities/entities.h"
#include "../utils/StringUtils.h"
#include "../utils/Logger.h"


ManageUserScene::ManageUserScene(bool isAdmin)
{
    this->type = isAdmin ? MangeUserSceneTypeEnum::CREATE_ADMIN_USER : MangeUserSceneTypeEnum::CREATE_USER;
    this->step = MangeUserSceneStepEnum::LOGIN;

    this->calculateView();
}

ManageUserScene::ManageUserScene(std::shared_ptr<User> user)
{
    this->type = MangeUserSceneTypeEnum::MODIFY_USER;
    this->step = MangeUserSceneStepEnum::PASSWORD;

    this->userLogin = user->getUserLogin();
    this->hashedPassword = user->getHashedPassword();
    this->email = user->getEmail();
    this->phone = user->getPhone();

    this->calculateView();
}


ManageUserScene::~ManageUserScene()
{
    Logger::log("ManageUserScene", "Destructor");
}

BaseScene * ManageUserScene::handleInput(StateManager& stateManager, const std::string& input) noexcept
{

    std::shared_ptr<User> tmp;
    switch (this->step)
    {
        case(MangeUserSceneStepEnum::LOGIN):
            tmp = stateManager.getUser(input);
            if (tmp.get() == nullptr)
            {
                this->userLogin = input;
                this->step = MangeUserSceneStepEnum::PASSWORD;
            }
            else
            {
                this->view.assign( { "User з даним Login вже існує\n", "Введіть User Login: " });
                return this;
            }
            break;
        case(MangeUserSceneStepEnum::PASSWORD):
            this->hashedPassword = StringUtils::hash(input);
            this->step = MangeUserSceneStepEnum::EMAIL;
            break;
        case(MangeUserSceneStepEnum::EMAIL):
            if (StringUtils::isValidEmail(input))
            {
                this->email = input;
                this->step = MangeUserSceneStepEnum::PHONE;
            }
            else
            {
                this->view.assign( { "Ви ввели некоректний email\n", "Введіть email: " });
                return this;
            }
            break;
        case(MangeUserSceneStepEnum::PHONE):
            if (StringUtils::isValidPhone(input))
            {
                this->phone = input;
                return this->complete(stateManager);
            }
            else
            {
                this->view.assign( { "Ви ввели некоректний номер телефону\n", "Введіть номер телефону: " });
                return this;
            }
    }

    this->calculateView();
    return this;
}

void ManageUserScene::calculateView()
{
    switch (this->step)
    {
        case(MangeUserSceneStepEnum::LOGIN):
            this->view.assign( { "Введіть User Login: " });
            break;
        case(MangeUserSceneStepEnum::PASSWORD):
            this->view.assign( { "Введіть пароль: " });
            break;
        case(MangeUserSceneStepEnum::EMAIL):
            if (this->email.empty())
            {
                this->view.assign( { "Введіть email: " });
            }
            else
            {
                this->view.assign( { "Введіть email (" + this->email + "): " });
            }
            break;
        case(MangeUserSceneStepEnum::PHONE):
            if (this->phone.empty())
            {
                this->view.assign( { "Введіть телефон: " });
            }
            else
            {
                this->view.assign( { "Введіть номер телефону (" + this->phone + "): " });
            }
            break;
    }
}

BaseScene * ManageUserScene::complete(StateManager& stateManager)
{
    if(this->type == MangeUserSceneTypeEnum::MODIFY_USER)
    {
        stateManager.modifyUser(this->userLogin, this->hashedPassword, this->email, this->phone);
        return new UserPageScene(stateManager.getUser(this->userLogin));
    }

    try
    {
        bool isAdmin = this->type == MangeUserSceneTypeEnum::CREATE_ADMIN_USER;
        std::shared_ptr<User> u { new User(this->userLogin, this->hashedPassword, this->email, this->phone, isAdmin) };
        stateManager.addUser(u);
    }
    catch (invalid_path e)
    {
        return new ErrorScene(e.what());
    }

    if (stateManager.isCurrentUserAdmin())
    {
        return new AdminMenuScene();
    }
    else
    {
        return new UnauthorizedMenuScene();
    }
}
