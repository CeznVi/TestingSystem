#include <string>
#include <memory>
#include <map>
#include <iostream>
#include <fstream>
#include "FileLoader.h"
#include "entities/entities.h"
#include "utils/StringUtils.h"
#include "utils/Logger.h"


enum EnitiesEnum
{
    ANSWER,
    QUESTION,
    TEST,
    USER,
    TESTREPORT,
};

std::map<std::string, EnitiesEnum> s_mapEnitiesEnumValues = {
    {"ANSWER", ANSWER},
    {"QUESTION", QUESTION},
    {"TEST", TEST},
    {"USER", USER},
    {"TESTREPORT", TESTREPORT},
};


file_not_exist::file_not_exist(const std::string& filePath)
{
    this->filePath = std::string(filePath);
}

const char* file_not_exist::what() const noexcept
{
    return ("File not exist at path: \"" + this->filePath + "\"").c_str();
}

invalid_path::invalid_path(const std::string& filePath)
{
    this->filePath = std::string(filePath);
}

const char* invalid_path::what() const noexcept
{
    return ("Given path are invalid: \"" + this->filePath + "\"").c_str();
}

void FileLoader::writeFile(std::string filePath, const EntitiesBatch& eb)
{
    std::ofstream f(filePath);

    if (f.fail())
    {
        throw invalid_path(filePath);
    }

    for (auto it = eb.answers.begin(); it != eb.answers.end(); ++it)
    {
        f << "ANSWER|" << (*it)->toString() << std::endl;
    }

    for (auto it = eb.questions.begin(); it != eb.questions.end(); ++it)
    {
        f << "QUESTION|" << (*it)->toString() << std::endl;
    }

    for (auto it = eb.tests.begin(); it != eb.tests.end(); ++it)
    {
        f << "TEST|" << (*it)->toString() << std::endl;
    }

    for (auto it = eb.users.begin(); it != eb.users.end(); ++it)
    {
        f << "USER|" << (*it)->toString() << std::endl;
    }

    for (auto it = eb.testReports.begin(); it != eb.testReports.end(); ++it)
    {
        f << "TESTREPORT|" << (*it)->toString() << std::endl;
    }

    f.close();
}

EntitiesBatch FileLoader::readFile(std::string filePath)
{
    std::ifstream f(filePath);

    if (f.fail())
    {
        throw file_not_exist(filePath);
    }

    EntitiesBatch result;
    std::map<unsigned int, std::shared_ptr<Question>> questions_map;
    std::map<unsigned int, std::shared_ptr<Test>> tests_map;

    std::shared_ptr<Question> tmp_q;
    std::shared_ptr<Test> tmp_t;

    std::string line;
    while (getline(f, line))
    {
        if (line.empty())
            continue;

        std::list<std::string> arguments = StringUtils::split(line, "|");
        std::string entityName = arguments.front();
        arguments.pop_front();

        switch (s_mapEnitiesEnumValues[entityName])
        {
        case (EnitiesEnum::ANSWER):
            result.answers.push_back(std::shared_ptr<Answer> {new Answer(arguments)});
            break;
        case (EnitiesEnum::QUESTION):
            tmp_q.reset(new Question(arguments));
            questions_map[tmp_q->getQuestionId()] = tmp_q;
            result.questions.push_back(tmp_q);
            break;
        case (EnitiesEnum::TEST):
            tmp_t.reset(new Test(arguments));
            tests_map[tmp_t->getTestId()] = tmp_t;
            result.tests.push_back(tmp_t);
            break;
        case (EnitiesEnum::USER):
            result.users.push_back(std::shared_ptr<User> {new User(arguments)});
            break;
        case (EnitiesEnum::TESTREPORT):
            result.testReports.push_back(std::shared_ptr<TestReport> {new TestReport(arguments)});
            break;
        }

    }
    f.close();

    for (auto it = result.answers.begin(); it != result.answers.end(); ++it)
    {
        questions_map[(*it)->getQuestionId()]->addAnswer((*it));
    }

    for (auto it = result.questions.begin(); it != result.questions.end(); ++it)
    {
        tests_map[(*it)->getTestId()]->addQuestion((*it));
    }

    return result;
}

void FileLoader::writeTestReportToFile(std::string filePath, std::shared_ptr<TestReport> testReport)
{
    std::ofstream f(filePath);

    if (f.fail())
    {
        throw invalid_path(filePath);
    }

    f << "\tРезультати тесту \"" + testReport->getTestName() + "\" для користувача " + testReport->getUserLogin() << std::endl;
    f << "Кількість баллів: " + std::to_string(testReport->getPoint()) + " / 12" << std::endl;
    f << "Час заверешненя тестування: " + testReport->getComlitionDateTime() << std::endl;

    f.close();
}
