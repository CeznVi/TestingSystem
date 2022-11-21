#include <string>
#include <list>
#include <set>
#include "entities.h"
#include "../utils/Logger.h"
#include <stdexcept>


template <typename Answer> unsigned int UniqueIdHelper<Answer>::prevMaxId = 0;
template <typename Answer> std::set<unsigned int> UniqueIdHelper<Answer>::setOfIds{};


Answer::Answer(unsigned int questionId, std::string text)
{
    this->answerId = getNextValidId();
    this->questionId = questionId;
    this->text = text;
}

Answer::Answer(const std::list<std::string>& str)
{
    if (str.size() != 3) 
        throw assembly_entity_error("Answer", "Expected 3 arguments but got " + std::to_string(str.size()));
    try
    {
        auto it = str.begin();
        this->answerId = std::stoi(*it);
        registerId(this->answerId);
        ++it;
        this->questionId = std::stoi(*it);
        ++it;
        this->text = *it;
    }         
    catch (const std::invalid_argument& e)
    {
        throw assembly_entity_error("Answer", "Error while parsing one of arguments.");
    }
}

Answer::~Answer()
{
    Logger::log("Answer(id="+std::to_string(this->answerId)+")", "Destructor");
}

std::string Answer::toString() noexcept
{
    return std::to_string(this->answerId) + "|" + std::to_string(this->questionId) + "|" + this->text;
}

std::string Answer::renderInList() noexcept
{
    return this->text;
}

unsigned int Answer::getAnswerId()
{
    return this->answerId;
}

unsigned int Answer::getQuestionId()
{
    return this->questionId;
}

std::string Answer::getText()
{
    return this->text;
}
