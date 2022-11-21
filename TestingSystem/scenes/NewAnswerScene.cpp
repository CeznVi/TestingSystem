#include <string>
#include <memory>
#include "scenes.h"
#include "../entities/entities.h"
#include "../utils/Logger.h"


NewAnswerScene::NewAnswerScene()
{
    this->view.assign({ "Введіть текст варіанту відповіді:" });
}

NewAnswerScene::~NewAnswerScene()
{
    Logger::log("NewAnswerScene", "Destructor");
}

BaseScene * NewAnswerScene::handleInput(StateManager& stateManager, const std::string& input) noexcept
{
    auto amount = stateManager.getAmountOfTmpAnswers() - 1;
    stateManager.setAmountOfTmpAnswers(amount);
    auto question = stateManager.getTmpQuestion();
    question->addAnswer(
        std::shared_ptr<Answer> { new Answer(question->getQuestionId(), input) }
    );

    if (amount == 0)
        return new SetCorrectAnswerScene(question->getAnswers());
    else
        return new NewAnswerScene();

}
