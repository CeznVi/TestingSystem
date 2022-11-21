#include <string>
#include <memory>
#include "scenes.h"
#include "../entities/entities.h"
#include "../utils/Logger.h"


NewTestScene::NewTestScene()
{
    this->step = NewTestSceneStepEnum::SUBJECT;
    this->calculateView();
}

NewTestScene::~NewTestScene()
{
    Logger::log("NewTestScene", "Destructor");
}

BaseScene * NewTestScene::handleInput(StateManager& stateManager, const std::string& input) noexcept
{
    int tmpAmount;
    switch (this->step)
    {
        case(NewTestSceneStepEnum::SUBJECT):
            this->subject = input;
            this->step = NewTestSceneStepEnum::NAME;
            break;
        case(NewTestSceneStepEnum::NAME):
            this->name = input;
            this->step = NewTestSceneStepEnum::AMOUNT_OF_QUESTIONS;
            break;
        case(NewTestSceneStepEnum::AMOUNT_OF_QUESTIONS):
            try 
            {
                tmpAmount = std::stoi(input);
                if (tmpAmount < 2 || tmpAmount > 30)
                {
                    this->setWarning();
                    return this;
                }
                else
                {
                    stateManager.setAmountOfTmpQuestions(tmpAmount);
                    stateManager.setTmpTest(std::shared_ptr<Test> { new Test(this->subject, this->name) });
                    return new NewQuestionScene();
                }
            }
            catch (...)
            {
                this->setWarning();
                return this;
            }
    }

    this->calculateView();
    return this;
}

void NewTestScene::calculateView()
{
    switch (this->step)
    {
        case(NewTestSceneStepEnum::SUBJECT):
            this->view.assign({ "Введіть категорію теста:" });
            break;
        case(NewTestSceneStepEnum::NAME):
            this->view.assign({ "Введіть назву теста:" });
            break;
        case(NewTestSceneStepEnum::AMOUNT_OF_QUESTIONS):
            this->view.assign({ "Введіть кількість питань (питаннь повинно бути більша ніж 2 та менше 30):" });
            break;
    }
}
