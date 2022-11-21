#pragma once
#include <string>
#include <list>
#include <set>
#include <memory>
#include <ctime>
#include <exception>


class entities_error: public std::exception { };

class not_unique_id: public entities_error
{
    public:
    not_unique_id(const std::string& entity);
    const char* what() const noexcept override;

    private:
    std::string entity;
};

class not_unique_user_login: public entities_error
{
    public:
    const char* what() const noexcept override;
};

class assembly_entity_error: public entities_error
{
    public:
    assembly_entity_error(const std::string& entity, const std::string& message);
    const char* what() const noexcept override;

    private:
    std::string entity;
    std::string message;
};

class BaseEntityType
{
    public:
    virtual std::string toString() noexcept = 0; // Method for convert entity to file
    virtual std::string renderInList() noexcept = 0; // Method to show entity as element of list in scene
};

template <class T>
class UniqueIdHelper: public BaseEntityType
{
    protected:
    static std::set<unsigned int> setOfIds;
    static unsigned int prevMaxId;

    static unsigned int getNextValidId()
    {
        UniqueIdHelper<T>::prevMaxId = UniqueIdHelper<T>::prevMaxId + 1;
        UniqueIdHelper<T>::setOfIds.insert(UniqueIdHelper<T>::prevMaxId);
        return UniqueIdHelper<T>::prevMaxId;
    };
    static void registerId(unsigned int id)
    {
        if (UniqueIdHelper<T>::setOfIds.find(id) != UniqueIdHelper<T>::setOfIds.end())
            throw not_unique_id(typeid(T).name());
        UniqueIdHelper<T>::setOfIds.insert(id);
        if (id > UniqueIdHelper<T>::prevMaxId)
            UniqueIdHelper<T>::prevMaxId = id;
    };
};

class Answer: public UniqueIdHelper<Answer>
{
    public:
    Answer(unsigned int questionId, std::string text);
    Answer(const std::list<std::string>& str);
    ~Answer();
    std::string toString() noexcept;
    std::string renderInList() noexcept;
    unsigned int getAnswerId();
    unsigned int getQuestionId();
    std::string getText();

    private:
    unsigned int answerId;
    unsigned int questionId;
    std::string text;
};

class Question: public UniqueIdHelper<Question>
{
    public:
    Question(unsigned int testId, std::string text);
    Question(const std::list<std::string>& str);
    ~Question();
    std::string toString() noexcept;
    std::string renderInList() noexcept;
    void addAnswer(std::shared_ptr<Answer> answer);
    std::list<std::shared_ptr<Answer>> getAnswers();
    void removeAnswer(std::shared_ptr<Answer> answer);
    void removeAnswer(unsigned int answerId);
    // void setCorrectAnswer(std::shared_ptr<Answer> answer);
    void setCorrectAnswerId(unsigned int answerId);
    // std::shared_ptr<Answer> getCorrectAnswer();
    unsigned int getCorrectAnswerId();
    unsigned int getQuestionId();
    unsigned int getTestId();
    unsigned int getAmountOfAnswers();
    std::string getText();

    private:
    unsigned int questionId;
    unsigned int testId;
    std::string text;
    std::list<std::shared_ptr<Answer>> answers;
    unsigned int correctAnswerId;
};

class Test: public UniqueIdHelper<Test>
{
    public:
    Test(std::string subject, std::string name);
    Test(const std::list<std::string>& str);
    ~Test();
    std::string toString() noexcept;
    std::string renderInList() noexcept;
    void addQuestion(std::shared_ptr<Question> question);
    std::list<std::shared_ptr<Question>> getQuestions();
    void removeQuestion(std::shared_ptr<Question> question);
    void removeQuestion(unsigned int questionId);
    unsigned int getTestId();
    unsigned int getAmountOfQuestions();
    std::string getSubject();
    std::string getName();

    private:
    unsigned int testId;
    std::string subject;
    std::string name;
    std::list<std::shared_ptr<Question>> questions;
};

class User: public BaseEntityType
{
    public:
    User(std::string userLogin, std::string hashedPassword, std::string email, std::string phone, bool isAdmin);
    User(const std::list<std::string>& str);
    ~User();
    std::string toString() noexcept;
    std::string renderInList() noexcept;

    std::string getUserLogin() const;
    std::string getHashedPassword() const;
    std::string getEmail() const;
    std::string getPhone() const;
    bool getIsAdmin();
    void setUserLogin(std::string userLogin);
    void setHashedPassword(std::string hashedPassword);
    void setEmail(std::string email);
    void setPhone(std::string phone);

    private:
    std::string userLogin;
    std::string hashedPassword;
    std::string email;
    std::string phone;
    bool isAdmin;

    static std::set<std::string> setOfUserLogins;
    static void registerUserLogin(std::string userLogin);
};

class TestReport: public BaseEntityType
{
    public:
    TestReport(std::string userLogin, std::string testName, unsigned int testId);
    TestReport(const std::list<std::string>& str);
    ~TestReport();
    std::string toString() noexcept;
    std::string renderInList() noexcept;
    void addAnswer(std::shared_ptr<Answer> answer);
    void addAnswer(unsigned int answerId);
    void completeTest();
    bool isTestCompleted();
    void setPoint(unsigned int point);
    unsigned int getPoint();
    std::string getUserLogin();
    std::string getTestName();
    unsigned int getTestId();
    bool isAnswerPresent(unsigned int answerId);
    std::string getComlitionDateTime();

    private:
    std::string userLogin;
    std::string testName;
    unsigned int testId;
    unsigned int point;
    std::set<unsigned int> answerIds;
    std::time_t timeOfCompletion;
};

class Subject: public BaseEntityType
{
    public:
    Subject(std::string name);
    ~Subject();
    std::string toString() noexcept;
    std::string renderInList() noexcept;
    std::string getName();

    private:
    std::string name;
};

struct EntitiesBatch
{
    std::list<std::shared_ptr<Answer>> answers;
    std::list<std::shared_ptr<Question>> questions;
    std::list<std::shared_ptr<Test>> tests;
    std::list<std::shared_ptr<User>> users;
    std::list<std::shared_ptr<TestReport>> testReports;
};
