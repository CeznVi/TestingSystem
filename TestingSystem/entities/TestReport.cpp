//щоб не ругалось на локал тайм
#define _CRT_SECURE_NO_WARNINGS
#include <string>
#include <set>
#include <list>
#include <memory>
#include <time.h>
#include "entities.h"
#include "../utils/StringUtils.h"
#include "../utils/Logger.h"
#include <stdexcept>


TestReport::TestReport(std::string userLogin, std::string testName, unsigned int testId)
{
    this->userLogin = userLogin;
    this->testName = testName;
    this->testId = testId;
    this->timeOfCompletion = 0;
    this->point = 0;
}

TestReport::TestReport(const std::list<std::string>& str)
{
    if (str.size() != 6) 
        throw assembly_entity_error("TestReport", "Expected 6 arguments but got " + std::to_string(str.size()));
    try
    {
        auto it = str.begin();
        this->userLogin = *it;
        ++it;
        this->testName = *it;
        ++it;
        this->testId = std::stoi(*it);
        ++it;
        this->answerIds = StringUtils::toSetOfUInt(*it);
        ++it;
        this->timeOfCompletion = std::stoi(*it);
        ++it;
        this->point = std::stoi(*it);
    }         
    catch (const std::invalid_argument& e)
    {
        throw assembly_entity_error("Answer", "Error while parsing one of arguments.");
    }
}

TestReport::~TestReport()
{
    Logger::log("TestReport(userLogin="+this->userLogin+",testId="+std::to_string(this->testId)+")", "Destructor");
}

std::string TestReport::toString() noexcept
{
    return this->userLogin + "|" + this->testName + "|" + std::to_string(this->testId) + "|" + 
        StringUtils::fromSetOfUInt(this->answerIds) + "|" + std::to_string(this->timeOfCompletion) +
        "|" + std::to_string(this->point);
}

std::string TestReport::renderInList() noexcept
{
    if (this->timeOfCompletion != 0)
        return this->testName + " (" + this->userLogin + "): " + this->getComlitionDateTime();
    else
        return this->testName + " (" + this->userLogin + ")";
}

void TestReport::addAnswer(std::shared_ptr<Answer> answer)
{
    this->answerIds.insert(answer->getAnswerId());
}

void TestReport::addAnswer(unsigned int answerId)
{
    this->answerIds.insert(answerId);
}

void TestReport::completeTest()
{
    time(&this->timeOfCompletion);
}

bool TestReport::isTestCompleted()
{
    return this->timeOfCompletion != 0;
}

void TestReport::setPoint(unsigned int point)
{
    this->point = point;
}

unsigned int TestReport::getPoint()
{
    return this->point;
}

std::string TestReport::getUserLogin()
{
    return this->userLogin;
}

std::string TestReport::getTestName()
{
    return this->testName;
}

unsigned int TestReport::getTestId()
{
    return this->testId;
}

bool TestReport::isAnswerPresent(unsigned int answerId)
{
    return this->answerIds.find(answerId) != this->answerIds.end();
}

std::string TestReport::getComlitionDateTime()
{
    std::tm * ptm = std::localtime(&this->timeOfCompletion);
    char buffer[32];
    std::strftime(buffer, 32, "%Y-%m-%d %H:%M:%S", ptm);
    return buffer;
}
