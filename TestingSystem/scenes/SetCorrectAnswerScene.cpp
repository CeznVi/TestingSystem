#include <string>
#include <list>
#include <memory>
#include "scenes.h"
#include "../entities/entities.h"
#include "../utils/Logger.h"


SetCorrectAnswerScene::SetCorrectAnswerScene(std::list<std::shared_ptr<Answer>> answers)
{
    this->answers = answers;
    this->view.assign({ "Оберіть варіант відповіді який буде рахуватися як коректний\n" });
    auto i = 0;
    for (auto it = this->answers.begin(); it != this->answers.end(); ++it, ++i)
    {
        this->view.push_back(std::to_string(i) + ") " + (**it).renderInList() + "\n");
    }
}

SetCorrectAnswerScene::~SetCorrectAnswerScene()
{
    Logger::log("SetCorrectAnswerScene", "Destructor");
}

BaseScene * SetCorrectAnswerScene::handleInput(StateManager& stateManager, const std::string& input) noexcept
{
    try
    {
        auto index = std::stoi(input);
        if (index >= 0 && index < this->answers.size())
        {
            auto it_a = this->answers.begin();
            std::advance(it_a, index);
            auto question = stateManager.getTmpQuestion();
            question->setCorrectAnswerId((*it_a)->getAnswerId());

            auto amount = stateManager.getAmountOfTmpQuestions() - 1;
            stateManager.setAmountOfTmpQuestions(amount);

            if (amount == 0)
            {
                auto test = stateManager.getTmpTest();
                auto questions = test->getQuestions();
                for (auto it_q = questions.begin(); it_q != questions.end(); ++it_q)
                {
                    stateManager.addQuestion(*it_q);
                    auto answers = (*it_q)->getAnswers();
                    for (auto it = answers.begin(); it != answers.end(); ++it)
                    {
                        stateManager.addAnswer(*it);
                    }
                }

                stateManager.addTest(test);

                return new MessageScene("Тест успішно створенно", new AdminTestManagementMenuScene());
            }
            else
                return new NewQuestionScene();
        }
    }
    catch(...) { }

    this->setWarning();
    return this;
}
