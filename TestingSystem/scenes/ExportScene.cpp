#include <list>
#include <set>
#include <memory>
#include <string>
#include "scenes.h"
#include "../FileLoader.h"
#include "../entities/entities.h"
#include "../utils/Logger.h"


ExportScene::ExportScene(std::shared_ptr<Subject> subject)
{
    this->subjectToExport = subject;
    this->initView();
}

ExportScene::ExportScene(std::shared_ptr<Test> test)
{
    this->testToExport = test;
    this->initView();
}

ExportScene::~ExportScene()
{
    Logger::log("ExportScene", "Destructor");
}

BaseScene * ExportScene::handleInput(StateManager& stateManager, const std::string& input) noexcept
{
    std::list<std::shared_ptr<Test>> tests;
    if (this->testToExport.get() != nullptr)
    {
        tests.push_back(this->testToExport);
    }
    else if (this->subjectToExport.get() != nullptr)
    {
        auto allTests = stateManager.getTests();
        auto subjectName = this->subjectToExport->getName();
        for (auto it = allTests.begin(); it != allTests.end(); ++it)
        {
            if ((*it)->getSubject() == subjectName)
            {
                tests.push_back((*it));
            }
        }
    }

    try
    {
        FileLoader::writeFile(input, this->prepareToExport(stateManager, tests));
    }
    catch (invalid_path e)
    {
        return new AdminTestManagementMenuScene(e.what());
    }
    return new AdminTestManagementMenuScene("Файл збережено");
}

void ExportScene::initView()
{
    this->view.assign({ "Введіть шлях для збереження:" });
}

EntitiesBatch ExportScene::prepareToExport(StateManager& stateManager, std::list<std::shared_ptr<Test>> tests)
{
    EntitiesBatch result;
    std::set<unsigned int> testIds;
    std::set<unsigned int> questionIds;
    result.tests = tests;

    for (auto it = tests.begin(); it != tests.end(); ++it)
    {
        testIds.insert((*it)->getTestId());
    }

    auto questions = stateManager.getQuestions();
    for (auto it = questions.begin(); it != questions.end(); ++it)
    {
        if ( testIds.find((*it)->getTestId()) != testIds.end() )
        {
            result.questions.push_back(*(it));
            questionIds.insert((*it)->getQuestionId());
        }
    }

    auto answers = stateManager.getAnswers();
    for (auto it = answers.begin(); it != answers.end(); ++it)
    {
        if ( questionIds.find((*it)->getQuestionId()) != questionIds.end() )
        {
            result.answers.push_back(*(it));
        }
    }

    return result;
}
