#include <string>
#include <memory>
#include "scenes.h"
#include "../entities/entities.h"
#include "../utils/Logger.h"


UserTestQuestionScene::UserTestQuestionScene(std::shared_ptr<Test> test)
{
    this->test = test;
    auto questions = this->test->getQuestions();
    this->currentQuestionIndex = 0;

    this->calculateView();
}

UserTestQuestionScene::UserTestQuestionScene(std::shared_ptr<Test> test, std::shared_ptr<TestReport> testReport)
{
    this->testReport = testReport;
    this->test = test;
    this->currentQuestionIndex = 0;
    auto questions = this->test->getQuestions();
    bool tmp_present;
    for (auto it_q = questions.begin(); it_q != questions.end(); ++it_q, ++this->currentQuestionIndex)
    {
        auto answers = (*it_q)->getAnswers();
        tmp_present = false;
        for (auto it_a = answers.begin(); it_a != answers.end(); ++it_a)
        {
            if (this->testReport->isAnswerPresent((*it_a)->getAnswerId()))
            {
                tmp_present = true;
                break;
            }
        }
        if (!tmp_present)
            break;
    }

    this->calculateView();
}


UserTestQuestionScene::~UserTestQuestionScene()
{
    Logger::log("UserTestQuestionScene", "Destructor");
}

BaseScene * UserTestQuestionScene::handleInput(StateManager& stateManager, const std::string& input) noexcept
{
    if (this->testReport.get() == nullptr)
    {
        this->testReport = std::shared_ptr<TestReport> { 
            new TestReport(stateManager.getCurrentUser()->getUserLogin(), this->test->getName(), this->test->getTestId())
        };
        stateManager.addTestReport(this->testReport);
    }

    if (input == "b")
    {
        return new UserMenuScene();
    }
    else
    {
        try
        {
            auto tmp_questions = this->test->getQuestions();
            auto tmp = tmp_questions.begin();
            std::advance(tmp, this->currentQuestionIndex);
            auto question = *tmp;
            auto index = std::stoi(input);

            auto answers = question->getAnswers();
            if (index >= 0 && index < answers.size())
            {
                auto it = answers.begin();
                std::advance(it, index);
                stateManager.addAnswerToTestReport(this->testReport, *it);

                this->currentQuestionIndex = this->currentQuestionIndex + 1;
                if (this->currentQuestionIndex == this->test->getAmountOfQuestions())
                {
                    stateManager.completeTest(this->testReport, this->test);
                    return new TestResultScene(this->testReport);
                }

                this->calculateView();
                return this;
            }
        }
        catch (...) { }
    }

    this->setWarning();
    return this;
}

void UserTestQuestionScene::calculateView()
{
    auto tmp_questions = this->test->getQuestions();
    auto tmp = tmp_questions.begin();
    std::advance(tmp, this->currentQuestionIndex);

    auto question = *tmp;
    auto answers = question->getAnswers();
    this->view.assign({ question->getText(), "\nОберіть відповідь:\n" });

    auto i = 0;
    for (auto it = answers.begin(); it != answers.end(); ++it, ++i)
    {
        this->view.push_back(std::to_string(i) + ") " + (*it)->getText() + "\n");
    }

    this->view.push_back("Введіть 'b', щоб повернутися до голоного меню (ви зможете повернутися до проходження тесту у будь який час)\n" );
}
