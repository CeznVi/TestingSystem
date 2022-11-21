#pragma once
#include <string>
#include <list>
#include <iterator>
#include <memory>
#include "../StateManager.h"
#include "../entities/entities.h"


class BaseScene
{   
    public:
    virtual ~BaseScene() {};
    virtual BaseScene* handleInput(StateManager& stateManager, const std::string& input) noexcept = 0;

    const std::list<std::string>* const getView() noexcept;

    protected:
    void setWarning();

    bool warning = false;
    std::list<std::string> view;
};

template <class EntityScene, class StepBackScene, class E>
class PaginationScene: public BaseScene
{
    public:
    PaginationScene(std::list<std::shared_ptr<E>> entityList);
    ~PaginationScene();
    BaseScene* handleInput(StateManager& stateManager, const std::string& input) noexcept;

    private:
    static const unsigned int MAX_PER_PAGE = 10;

    void calculateView();
    virtual BaseScene* processEnitity(std::shared_ptr<E> entity, StateManager& stateManager);

    std::list<std::shared_ptr<E>> entityList;
    unsigned int currentPage;
    unsigned int totalPages;
};

template <class E>
class DummyScene: public BaseScene
{
    public:
    DummyScene(std::shared_ptr<E> entity) { };
    ~DummyScene() {};
    BaseScene* handleInput(StateManager& stateManager, const std::string& input) noexcept { return nullptr; }
};

enum TestResultSceneModeEnum
{ 
    VIEW, SAVE_TO_FILE
};

class TestResultScene: public BaseScene
{
    public:
    TestResultScene(std::shared_ptr<TestReport> testReport);
    ~TestResultScene();
    BaseScene* handleInput(StateManager& stateManager, const std::string& input) noexcept;\

    private:
    void calculateView();
    TestResultSceneModeEnum mode;
    std::shared_ptr<TestReport> testReport;
};

enum MangeUserSceneTypeEnum
{ 
    CREATE_USER, CREATE_ADMIN_USER, MODIFY_USER
};

enum MangeUserSceneStepEnum
{
    LOGIN, PASSWORD, EMAIL, PHONE
};

class ManageUserScene: public BaseScene
{
    public:
    ManageUserScene(bool isAdmin = false);
    ManageUserScene(std::shared_ptr<User> user);
    ~ManageUserScene();
    BaseScene* handleInput(StateManager& stateManager, const std::string& input) noexcept;

    private:
    void calculateView();
    BaseScene* complete(StateManager& stateManager);

    MangeUserSceneTypeEnum type;
    MangeUserSceneStepEnum step;

    std::string userLogin;
    std::string hashedPassword;
    std::string email;
    std::string phone;
};

class InitScene: public BaseScene
{
    public:
    InitScene();
    ~InitScene();
    BaseScene* handleInput(StateManager& stateManager, const std::string& input) noexcept;
};

class ErrorScene: public BaseScene
{
    public:
    ErrorScene(const std::string& errorMessage);
    ~ErrorScene();
    BaseScene* handleInput(StateManager& stateManager, const std::string& input) noexcept;
};

class MessageScene: public BaseScene
{
    public:
    MessageScene(const std::string& message, BaseScene* nextScene);
    ~MessageScene();
    BaseScene* handleInput(StateManager& stateManager, const std::string& input) noexcept;

    private:
    std::unique_ptr<BaseScene> nextScene;
};

class UnauthorizedMenuScene: public BaseScene
{
    public:
    UnauthorizedMenuScene();
    UnauthorizedMenuScene(std::string message);
    ~UnauthorizedMenuScene();
    BaseScene* handleInput(StateManager& stateManager, const std::string& input) noexcept;

    private:
    void initView();
};

class LoginScene: public BaseScene
{
    public:
    LoginScene();
    ~LoginScene();
    BaseScene* handleInput(StateManager& stateManager, const std::string& input) noexcept;

    private:
    std::string userLogin;
    std::string hashedPassword;
};

class UserMenuScene: public BaseScene
{
    public:
    UserMenuScene();
    UserMenuScene(std::string message);
    ~UserMenuScene();
    BaseScene* handleInput(StateManager& stateManager, const std::string& input) noexcept;

    private:
    void initView();
};

class UserTestQuestionScene: public BaseScene
{
    public:
    UserTestQuestionScene(std::shared_ptr<Test> test);
    UserTestQuestionScene(std::shared_ptr<Test> test, std::shared_ptr<TestReport> testReport);
    ~UserTestQuestionScene();
    BaseScene* handleInput(StateManager& stateManager, const std::string& input) noexcept;

    const std::list<std::string>* const getView() noexcept;

    private:
    void calculateView();

    std::shared_ptr<Test> test;
    std::shared_ptr<TestReport> testReport;

    unsigned int currentQuestionIndex;
};

class UserSubjectListScene:
    public PaginationScene<DummyScene<Subject>, UserMenuScene, Subject>
{
    public:
    UserSubjectListScene(std::list<std::shared_ptr<Subject>> subjects):
        PaginationScene<DummyScene<Subject>, UserMenuScene, Subject>(subjects) { };
    private:
    BaseScene* processEnitity(std::shared_ptr<Subject> entity, StateManager& stateManager);
};

class UserUnfinishedTestListScene:
    public PaginationScene<DummyScene<TestReport>, UserMenuScene, TestReport>
{
    public:
    UserUnfinishedTestListScene(std::list<std::shared_ptr<TestReport>> testReports):
        PaginationScene<DummyScene<TestReport>, UserMenuScene, TestReport>(testReports) { };
    private:
    BaseScene* processEnitity(std::shared_ptr<TestReport> testReport, StateManager& stateManager);
};

class AdminMenuScene: public BaseScene
{
    public:
    AdminMenuScene();
    AdminMenuScene(std::string message);
    ~AdminMenuScene();
    BaseScene* handleInput(StateManager& stateManager, const std::string& input) noexcept;

    private:
    void initView();
};

class AdminTestManagementMenuScene: public BaseScene
{
    public:
    AdminTestManagementMenuScene();
    AdminTestManagementMenuScene(std::string message);

    ~AdminTestManagementMenuScene();
    BaseScene* handleInput(StateManager& stateManager, const std::string& input) noexcept;

    private:
    void initView();
};

class ViewResultMenuScene: public BaseScene
{
    public:
    ViewResultMenuScene();
    ViewResultMenuScene(std::string message);

    ~ViewResultMenuScene();
    BaseScene* handleInput(StateManager& stateManager, const std::string& input) noexcept;

    private:
    void initView();
};

class UsersListForResultsScene:
    public PaginationScene<DummyScene<User>, ViewResultMenuScene, User>
{
    public:
    UsersListForResultsScene(std::list<std::shared_ptr<User>> users):
        PaginationScene<DummyScene<User>, ViewResultMenuScene, User>(users) { };
    private:
    BaseScene* processEnitity(std::shared_ptr<User> user, StateManager& stateManager);
};

class TestsListForResultsScene:
    public PaginationScene<DummyScene<Test>, ViewResultMenuScene, Test>
{
    public:
    TestsListForResultsScene(std::list<std::shared_ptr<Test>> tests):
        PaginationScene<DummyScene<Test>, ViewResultMenuScene, Test>(tests) { };
    private:
    BaseScene* processEnitity(std::shared_ptr<Test> test, StateManager& stateManager);
};

class SubjectsListForResultsScene:
    public PaginationScene<DummyScene<Subject>, ViewResultMenuScene, Subject>
{
    public:
    SubjectsListForResultsScene(std::list<std::shared_ptr<Subject>> subjects):
        PaginationScene<DummyScene<Subject>, ViewResultMenuScene, Subject>(subjects) { };
    private:
    BaseScene* processEnitity(std::shared_ptr<Subject> subject, StateManager& stateManager);
};

class NewSubjectScene: public BaseScene
{
    public:
    NewSubjectScene();
    ~NewSubjectScene();
    BaseScene* handleInput(StateManager& stateManager, const std::string& input) noexcept;
};

enum NewTestSceneStepEnum
{
    SUBJECT, NAME, AMOUNT_OF_QUESTIONS
};

class NewTestScene: public BaseScene
{
    public:
    NewTestScene();
    ~NewTestScene();
    BaseScene* handleInput(StateManager& stateManager, const std::string& input) noexcept;

    private:
    void calculateView();

    NewTestSceneStepEnum step;
    std::string subject;
    std::string name;
};

enum NewQuestionSceneStepEnum
{
    TEXT, AMOUNT_OF_ANSWER
};

class NewQuestionScene: public BaseScene
{
    public:
    NewQuestionScene();
    ~NewQuestionScene();
    BaseScene* handleInput(StateManager& stateManager, const std::string& input) noexcept;

    private:
    void calculateView();

    NewQuestionSceneStepEnum step;
    std::string text;
};

class NewAnswerScene: public BaseScene
{
    public:
    NewAnswerScene();
    ~NewAnswerScene();
    BaseScene* handleInput(StateManager& stateManager, const std::string& input) noexcept;
};

class SetCorrectAnswerScene: public BaseScene
{
    public:
    SetCorrectAnswerScene(std::list<std::shared_ptr<Answer>> answers);
    ~SetCorrectAnswerScene();
    BaseScene* handleInput(StateManager& stateManager, const std::string& input) noexcept;

    private:
    std::list<std::shared_ptr<Answer>> answers;
};

class ExportScene: public BaseScene
{
    public:
    ExportScene(std::shared_ptr<Subject> subject);
    ExportScene(std::shared_ptr<Test> test);

    ~ExportScene();
    BaseScene* handleInput(StateManager& stateManager, const std::string& input) noexcept;

    private:
    void initView();
    EntitiesBatch prepareToExport(StateManager& stateManager, std::list<std::shared_ptr<Test>> tests);

    std::shared_ptr<Subject> subjectToExport;
    std::shared_ptr<Test> testToExport;
};

class ImportScene: public BaseScene
{
    public:
    ImportScene();
    ~ImportScene();
    BaseScene* handleInput(StateManager& stateManager, const std::string& input) noexcept;
};

class UserPageScene: public BaseScene
{
    public:
    UserPageScene(std::shared_ptr<User> user);
    ~UserPageScene();
    BaseScene* handleInput(StateManager& stateManager, const std::string& input) noexcept;

    private:
    std::shared_ptr<User> user;
};

class ChangePasswordScene: public BaseScene
{
    public:
    ChangePasswordScene();
    ~ChangePasswordScene();
    BaseScene* handleInput(StateManager& stateManager, const std::string& input) noexcept;
};
