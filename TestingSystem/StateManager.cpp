#include <string>
#include <memory>
#include <algorithm>
#include "StateManager.h"
#include "FileLoader.h"
#include "entities/entities.h"
#include "utils/Logger.h"


StateManager::StateManager()
{
    Logger::log("StateManager", "Constructor");
}

StateManager::~StateManager()
{
    Logger::log("StateManager", "Destructor");
}

void StateManager::initState(std::string filePath)
{
    this->filePath = filePath;
    this->state = FileLoader::readFile(filePath);
    this->isAdminUserPresent = std::any_of(
        this->state.users.begin(), this->state.users.end(),
        [](std::shared_ptr<User> u) {return u->getIsAdmin(); }
    );

    for (auto it = this->state.tests.begin(); it != this->state.tests.end(); ++it)
    {
        std::string tmpSubject = (**it).getSubject();
        if (this->subjectsName.find(tmpSubject) == this->subjectsName.end())
        {
            this->subjectsName.insert(tmpSubject);
            this->subjects.push_back(std::shared_ptr<Subject> { new Subject(tmpSubject) });
        }
    }
}

void StateManager::addUser(std::shared_ptr<User> user)
{
    this->state.users.push_back(user);
    this->sync();
}

void StateManager::modifyUser(std::string userLogin, std::string hashedPassword, std::string email, std::string phone)
{
    auto user = this->getUser(userLogin);

    if (user.get() != nullptr)
    {
        user->setHashedPassword(hashedPassword);
        user->setEmail(email);
        user->setPhone(phone);
    }

    this->sync();
}

void StateManager::deleteUser(std::shared_ptr<User> user)
{
    auto userLogin = user->getUserLogin();
    auto it = std::find_if(
        this->state.users.begin(), this->state.users.end(),
        [&userLogin](std::shared_ptr<User> u) { return u->getUserLogin() == userLogin; }
    );

    if (it != this->state.users.end())
    {
        this->state.users.erase(it);
        this->sync();
    }
}

std::shared_ptr<User> StateManager::getUser(std::string userLogin)
{
    auto it = std::find_if(
        this->state.users.begin(), this->state.users.end(),
        [&userLogin](std::shared_ptr<User> u) { return u->getUserLogin() == userLogin; }
    );

    if (it != this->state.users.end())
    {
        return (*it);
    }
    else
    {
        return std::shared_ptr<User> { nullptr };
    }
}

std::list<std::shared_ptr<User>> StateManager::getUsers()
{
    return this->state.users;
}

void StateManager::setCurrentUser(std::shared_ptr<User> user)
{
    this->currentUser = user;
}

std::shared_ptr<User> StateManager::getCurrentUser()
{
    return this->currentUser;
}

bool StateManager::getIsAdminUserPresent()
{
    return this->isAdminUserPresent;
}

bool StateManager::isCurrentUserAdmin()
{
    if (this->currentUser.get() != nullptr)
    {
        return this->currentUser->getIsAdmin();
    }
    else
    {
        return false;
    }
}

void StateManager::addSubject(std::string subjectName)
{
    if (this->subjectsName.find(subjectName) == this->subjectsName.end())
    {
        this->subjectsName.insert(subjectName);
        this->subjects.push_back(std::shared_ptr<Subject> { new Subject(subjectName) });
    }
}

std::list<std::shared_ptr<Subject>> StateManager::getSubjects()
{
    return this->subjects;
}

std::list<std::shared_ptr<Test>> StateManager::getTests()
{
    return this->state.tests;
}

std::shared_ptr<Test> StateManager::getTestById(unsigned int testId)
{
    for (auto it = this->state.tests.begin(); it != this->state.tests.end(); ++it)
    {
        if ((*it)->getTestId() == testId)
            return *it;
    }

    return std::shared_ptr<Test> { nullptr };
}


void StateManager::addTest(std::shared_ptr<Test> test)
{
    this->state.tests.push_back(test);
    this->addSubject(test->getSubject());
    this->sync();
}

std::list<std::shared_ptr<Question>> StateManager::getQuestions()
{
    return this->state.questions;
}

void StateManager::addQuestion(std::shared_ptr<Question> question)
{
    this->state.questions.push_back(question);
    this->sync();
}

std::list<std::shared_ptr<Answer>> StateManager::getAnswers()
{
    return this->state.answers;
}

std::list<std::shared_ptr<TestReport>> StateManager::getTestReports()
{
    return this->state.testReports;
}

std::list<std::shared_ptr<TestReport>> StateManager::getTestReportsByUser(std::string userLogin)
{
    std::list<std::shared_ptr<TestReport>> results;

    for (auto it = this->state.testReports.begin(); it != this->state.testReports.end(); ++it)
    {
        if ((*it)->getUserLogin() == userLogin)
            results.push_front(*it);
    }

    return results;
}

void StateManager::addTestReport(std::shared_ptr<TestReport> testReport)
{
    this->state.testReports.push_back(testReport);
    this->sync();
}

void StateManager::addAnswerToTestReport(std::shared_ptr<TestReport> testReport, std::shared_ptr<Answer> answer)
{
    testReport->addAnswer(answer->getAnswerId());
    this->sync();
}

void StateManager::completeTest(std::shared_ptr<TestReport> testReport, std::shared_ptr<Test> test)
{
    testReport->completeTest();
    auto questions = test->getQuestions();
    unsigned int correctAnswers = 0;
    for (auto it = questions.begin(); it != questions.end(); ++it)
    {
        if (testReport->isAnswerPresent((*it)->getCorrectAnswerId()))
            ++correctAnswers;
    }
    Logger::log("StateManager::completeTest", std::to_string(correctAnswers));
    Logger::log("StateManager::completeTest", std::to_string(test->getAmountOfQuestions()));

    Logger::log("StateManager::completeTest", std::to_string(((double)correctAnswers / (double)test->getAmountOfQuestions())));

    testReport->setPoint(
        ((double)correctAnswers / (double)test->getAmountOfQuestions()) * 12
    );

    this->sync();
}

void StateManager::addAnswer(std::shared_ptr<Answer> answer)
{
    this->state.answers.push_back(answer);
    this->sync();
}

void StateManager::sync()
{
    FileLoader::writeFile(this->filePath, this->state);
}

void StateManager::setTmpTest(std::shared_ptr<Test> test)
{
    this->tmpTest = test;
}

std::shared_ptr<Test> StateManager::getTmpTest()
{
    return this->tmpTest;
}

void StateManager::setAmountOfTmpQuestions(unsigned int amount)
{
    this->amountOfTmpQuestions = amount;
}

unsigned int StateManager::getAmountOfTmpQuestions()
{
    return this->amountOfTmpQuestions;
}

void StateManager::setTmpQuestion(std::shared_ptr<Question> question)
{
    this->tmpQuestion = question;
}

std::shared_ptr<Question> StateManager::getTmpQuestion()
{
    return this->tmpQuestion;
}

void StateManager::setAmountOfTmpAnswers(unsigned int amount)
{
    this->amountOfTmpAnswers = amount;
}

unsigned int StateManager::getAmountOfTmpAnswers()
{
    return this->amountOfTmpAnswers;
}
