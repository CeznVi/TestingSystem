#include <string>
#include <set>
#include <list>
#include <memory>
#include "entities.h"
#include "../utils/Logger.h"
#include <stdexcept>


template <typename Test> unsigned int UniqueIdHelper<Test>::prevMaxId = 0;
template <typename Test> std::set<unsigned int> UniqueIdHelper<Test>::setOfIds{};


Test::Test(std::string subject, std::string name)
{
    this->testId = getNextValidId();
    this->subject = subject;
    this->name = name;
}

Test::Test(const std::list<std::string>& str)
{
    if (str.size() != 3) 
        throw assembly_entity_error("Test", "Expected 3 arguments but got " + std::to_string(str.size()));
    try
    {
        auto it = str.begin();
        this->testId = std::stoi(*it);
        registerId(this->testId);
        ++it;
        this->subject = *it;
        ++it;
        this->name = *it;
    }         
    catch (const std::invalid_argument& e)
    {
        throw assembly_entity_error("Answer", "Error while parsing one of arguments.");
    }
}

Test::~Test()
{
    Logger::log("Test(id="+std::to_string(this->testId)+")", "Destructor");
}

std::string Test::toString() noexcept
{
    return std::to_string(this->testId) + "|" + this->subject + "|" + this->name;
}

std::string Test::renderInList() noexcept
{
    return this->name;
}

void Test::addQuestion(std::shared_ptr<Question> question)
{
    this->questions.push_back(question);
}

std::list<std::shared_ptr<Question>> Test::getQuestions()
{
    return this->questions;
}

void Test::removeQuestion(std::shared_ptr<Question> question)
{
    this->questions.remove(question);
}

void Test::removeQuestion(unsigned int questionId)
{
    this->questions.remove_if([&questionId](std::shared_ptr<Question> a) { return a.get()->getQuestionId() == questionId; });
}

unsigned int Test::getTestId()
{
    return this->testId;
}

unsigned int Test::getAmountOfQuestions()
{
    return this->questions.size();
}

std::string Test::getSubject()
{
    return this->subject;
}

std::string Test::getName()
{
    return this->name;
}
