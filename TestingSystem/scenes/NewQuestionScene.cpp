#include <string>
#include "scenes.h"
#include "../entities/entities.h"
#include "../utils/Logger.h"


NewQuestionScene::NewQuestionScene()
{
    this->step = NewQuestionSceneStepEnum::TEXT;
    this->calculateView();
}

NewQuestionScene::~NewQuestionScene()
{
    Logger::log("NewQuestionScene", "Destructor");
}

BaseScene * NewQuestionScene::handleInput(StateManager& stateManager, const std::string& input) noexcept
{
    int tmpAmount;

    switch (this->step)
    {
        case(NewQuestionSceneStepEnum::TEXT):
            this->text = input;
            this->step = NewQuestionSceneStepEnum::AMOUNT_OF_ANSWER;
            break;
        case(NewQuestionSceneStepEnum::AMOUNT_OF_ANSWER):
            try
            {
                tmpAmount = std::stoi(input);
                if (tmpAmount < 2 || tmpAmount > 10)
                {
                    this->setWarning();
                    return this;
                }
                else
                {
                    stateManager.setAmountOfTmpAnswers(tmpAmount);
                    stateManager.setTmpQuestion(std::shared_ptr<Question> { 
                        new Question(stateManager.getTmpTest()->getTestId(), this->text) 
                    });
                    stateManager.getTmpTest()->addQuestion(stateManager.getTmpQuestion());
                    return new NewAnswerScene();
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

void NewQuestionScene::calculateView()
{
    switch (this->step)
    {
        case(NewQuestionSceneStepEnum::TEXT):
            this->view.assign({ "Введіть текст питання:" });
            break;
        case(NewQuestionSceneStepEnum::AMOUNT_OF_ANSWER):
            this->view.assign({ "Введіть кількість варіантів відповідей (від 2 до 10):" });
            break;
    }
}
