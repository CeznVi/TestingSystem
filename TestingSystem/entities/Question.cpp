#include <string>
#include <set>
#include <list>
#include <memory>
#include <algorithm>
#include "entities.h"
#include "../utils/Logger.h"
#include <stdexcept>


template <typename Question> unsigned int UniqueIdHelper<Question>::prevMaxId = 0;
template <typename Question> std::set<unsigned int> UniqueIdHelper<Question>::setOfIds{};


Question::Question(unsigned int testId, std::string text)
{
    this->questionId = getNextValidId();
    this->testId = testId;
    this->text = text;
    this->correctAnswerId = 0;
}

Question::Question(const std::list<std::string>& str)
{
    if (str.size() != 4) 
        throw assembly_entity_error("Question", "Expected 4 arguments but got " + std::to_string(str.size()));
    try
    {
        auto it = str.begin();
        this->questionId = std::stoi(*it);
        registerId(this->questionId);
        ++it;
        this->testId = std::stoi(*it);
        ++it;
        this->text = *it;
        ++it;
        this->correctAnswerId = std::stoi(*it);
    }         
    catch (const std::invalid_argument& e)
    {
        throw assembly_entity_error("Question", "Error while parsing one of arguments.");
    }
}

Question::~Question()
{
    Logger::log("Question(id="+std::to_string(this->questionId)+")", "Destructor");
}

std::string Question::toString() noexcept
{
    return std::to_string(this->questionId) + "|" + std::to_string(this->testId) + "|" + this->text + "|" + std::to_string(this->correctAnswerId);
}

std::string Question::renderInList() noexcept
{
    return this->text;
}

void Question::addAnswer(std::shared_ptr<Answer> answer)
{
    // TODO: Update questionId in given answer
    this->answers.push_back(answer);
}

std::list<std::shared_ptr<Answer>> Question::getAnswers()
{
    return this->answers;
}

void Question::removeAnswer(std::shared_ptr<Answer> answer)
{
    this->answers.remove(answer);
}

void Question::removeAnswer(unsigned int answerId)
{
    this->answers.remove_if([&answerId](std::shared_ptr<Answer> a) { return a.get()->getAnswerId() == answerId; });
}

void Question::setCorrectAnswerId(unsigned int answerId)
{
    // TODO: check if this answerId present in this->answers;
    this->correctAnswerId = answerId;
}

unsigned int Question::getCorrectAnswerId()
{
    return this->correctAnswerId;
}

unsigned int Question::getQuestionId()
{
    return this->questionId;
}

unsigned int Question::getTestId()
{
    return this->testId;
}

unsigned int Question::getAmountOfAnswers()
{
    return this->answers.size();
}

std::string Question::getText()
{
    return this->text;
}
