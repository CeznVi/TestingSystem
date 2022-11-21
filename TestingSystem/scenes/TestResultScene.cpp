#include <string>
#include <memory>
#include "scenes.h"
#include "../entities/entities.h"
#include "../FileLoader.h"
#include "../utils/Logger.h"


TestResultScene::TestResultScene(std::shared_ptr<TestReport> testReport)
{
    this->testReport = testReport;
    this->mode = TestResultSceneModeEnum::VIEW;

    this->calculateView();
}

TestResultScene::~TestResultScene()
{
    Logger::log("TestResultScene", "Destructor");
}

BaseScene * TestResultScene::handleInput(StateManager& stateManager, const std::string& input) noexcept
{
    std::string resultText;
    switch (this->mode)
    {
        case (TestResultSceneModeEnum::VIEW):
            if (input == "b")
            {
                if (stateManager.getCurrentUser()->getIsAdmin())
                    return new ViewResultMenuScene();
                else
                    return new UserMenuScene();
            }
            else if (input == "s")
            {
                this->mode = TestResultSceneModeEnum::SAVE_TO_FILE;
                this->calculateView();
                return this;
            }
        case (TestResultSceneModeEnum::SAVE_TO_FILE):
            try
            {
                FileLoader::writeTestReportToFile(input, this->testReport);
                resultText = "Картку резултату тесту було успішно збережено у файл";
            }
            catch (invalid_path e)
            {
                resultText = e.what();
            }
            if (stateManager.getCurrentUser()->getIsAdmin())
                return new ViewResultMenuScene(resultText);
            else
                return new UserMenuScene(resultText);
    }

    this->setWarning();
    return this;
}

void TestResultScene::calculateView()
{
    switch (this->mode)
    {
        case (TestResultSceneModeEnum::VIEW):
            this->view.assign({
                "\tРезультати тесту \"" + this->testReport->getTestName() + "\" для користувача " + this->testReport->getUserLogin(),
                "\nКількість баллів: " + std::to_string(this->testReport->getPoint()) + " / 12",
                "\nЧас заверешненя тестування: " + this->testReport->getComlitionDateTime(),
                "\nВведіть 's', щоб зберегти картку резултатів\n",
                "Введіть 'b', щоб повернутися\n"
            });
            break;
        case (TestResultSceneModeEnum::SAVE_TO_FILE):
            this->view.assign({
                "Введіть шлях для збереження: "
            });
            break;
    }
}
