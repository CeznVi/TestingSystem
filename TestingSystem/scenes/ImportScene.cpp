#include <string>
#include "scenes.h"
#include "../entities/entities.h"
#include "../utils/Logger.h"


ImportScene::ImportScene()
{
    this->view.assign({ "Введіть шлях до файлу з тестами:" });
}

ImportScene::~ImportScene()
{
    Logger::log("ImportScene", "Destructor");
}

BaseScene * ImportScene::handleInput(StateManager& stateManager, const std::string& input) noexcept
{
    try
    {
        EntitiesBatch eb = FileLoader::readFile(input);
        for (auto it = eb.tests.begin(); it != eb.tests.end(); ++it)
        {
            stateManager.addTest((*it));
        }
        for (auto it = eb.questions.begin(); it != eb.questions.end(); ++it)
        {
            stateManager.addQuestion((*it));
        }
        for (auto it = eb.answers.begin(); it != eb.answers.end(); ++it)
        {
            stateManager.addAnswer((*it));
        }
    }
    catch (file_not_exist e)
    {
        return new AdminTestManagementMenuScene(e.what());
    }
    catch (entities_error& e)
    {
        return new AdminTestManagementMenuScene(e.what()); 
    }
    

    return new AdminTestManagementMenuScene("Файл з тестами завантажено");
}
