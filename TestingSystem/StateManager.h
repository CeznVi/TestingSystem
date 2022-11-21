#pragma once
#include <string>
#include <memory>
#include <set>
#include <list>
#include "entities/entities.h"
#include "FileLoader.h"


class StateManager
{
public:
    StateManager();
    ~StateManager();

    void initState(std::string filePath);

    // User
    void addUser(std::shared_ptr<User> user);
    void modifyUser(std::string userLogin, std::string hashedPassword, std::string email, std::string phone);
    void deleteUser(std::shared_ptr<User> user);
    std::shared_ptr<User> getUser(std::string userLogin);
    std::list<std::shared_ptr<User>> getUsers();
    void setCurrentUser(std::shared_ptr<User> user);
    std::shared_ptr<User> getCurrentUser();

    bool getIsAdminUserPresent();
    bool isCurrentUserAdmin();

    // Subject
    void addSubject(std::string subjectName);
    std::list<std::shared_ptr<Subject>> getSubjects();

    // Test
    std::list<std::shared_ptr<Test>> getTests();
    std::shared_ptr<Test> getTestById(unsigned int testId);
    void addTest(std::shared_ptr<Test> test);

    // Question
    std::list<std::shared_ptr<Question>> getQuestions();
    void addQuestion(std::shared_ptr<Question> question);

    // Answer
    std::list<std::shared_ptr<Answer>> getAnswers();
    void addAnswer(std::shared_ptr<Answer> answer);

    // TestReport
    std::list<std::shared_ptr<TestReport>> getTestReports();
    std::list<std::shared_ptr<TestReport>> getTestReportsByUser(std::string userLogin);
    void addTestReport(std::shared_ptr<TestReport> testReport);
    void addAnswerToTestReport(std::shared_ptr<TestReport> testReport, std::shared_ptr<Answer> answer);
    void completeTest(std::shared_ptr<TestReport> testReport, std::shared_ptr<Test> test);

    // New test flow
    void setTmpTest(std::shared_ptr<Test> test);
    std::shared_ptr<Test> getTmpTest();
    void setAmountOfTmpQuestions(unsigned int amount);
    unsigned int getAmountOfTmpQuestions();
    void setTmpQuestion(std::shared_ptr<Question> question);
    std::shared_ptr<Question> getTmpQuestion();
    void setAmountOfTmpAnswers(unsigned int amount);
    unsigned int getAmountOfTmpAnswers();

private:
    void sync();

    std::string filePath;
    EntitiesBatch state;
    bool isAdminUserPresent;
    std::shared_ptr<User> currentUser;

    std::set<std::string> subjectsName;
    std::list<std::shared_ptr<Subject>> subjects;

    std::shared_ptr<Test> tmpTest;
    unsigned int amountOfTmpQuestions;
    std::shared_ptr<Question> tmpQuestion;
    unsigned int amountOfTmpAnswers;
};
