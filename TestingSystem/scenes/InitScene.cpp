#include "scenes.h"
#include "../FileLoader.h"
#include "../entities/entities.h"
#include "../utils/Logger.h"


InitScene::InitScene()
{
    this->view.assign( { "Введіть шлях до бази даних (якщо файла не існує його буде створенно): " } );
}

BaseScene * InitScene::handleInput(StateManager& stateManager, const std::string& input) noexcept
{
    try
    {
        stateManager.initState(input);
        if (stateManager.getIsAdminUserPresent())
        {
            return new UnauthorizedMenuScene();
        }
        else
        {
            return new ManageUserScene(true);
        }
    }
    catch(file_not_exist)
    {
        return new ManageUserScene(true);
    }
    catch(entities_error& e)
    {
        return new ErrorScene(e.what());
    }
}

InitScene::~InitScene()
{
    Logger::log("InitScene", "Destructor");
}