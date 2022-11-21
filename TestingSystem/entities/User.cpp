#include <string>
#include <list>
#include <set>
#include "entities.h"
#include "../utils/StringUtils.h"
#include "../utils/Logger.h"
#include <stdexcept>


std::set<std::string> User::setOfUserLogins{};
void User::registerUserLogin(std::string userLogin)
{
    if (setOfUserLogins.find(userLogin) != setOfUserLogins.end())
        throw not_unique_user_login();
    setOfUserLogins.insert(userLogin);
}

User::User(std::string userLogin, std::string hashedPassword, std::string email, std::string phone, bool isAdmin)
{
    this->userLogin = userLogin;
    User::registerUserLogin(this->userLogin);
    this->hashedPassword = hashedPassword;
    this->email = email;
    this->phone = phone;
    this->isAdmin = isAdmin;
}

User::User(const std::list<std::string>& str)
{
    if (str.size() != 5) 
        throw assembly_entity_error("User", "Expected 5 arguments but got " + std::to_string(str.size()));
    try
    {
        auto it = str.begin();
        this->userLogin = *it;
        User::registerUserLogin(this->userLogin);
        ++it;
        this->hashedPassword = *it;
        ++it;
        this->email = *it;
        ++it;
        this->phone = *it;
        ++it;
        this->isAdmin = StringUtils::toBool(*it);
    }         
    catch (const std::invalid_argument& e)
    {
        throw assembly_entity_error("User", "Error while parsing one of arguments.");
    }
}

User::~User()
{
    Logger::log("User(id="+this->userLogin+")", "Destructor");
}

std::string User::toString() noexcept
{
    return this->userLogin + "|" + this->hashedPassword + "|" + this->email + "|" + this->phone + "|" + StringUtils::fromBool(this->isAdmin);
}

std::string User::renderInList() noexcept
{
    return this->userLogin;
}

bool User::getIsAdmin()
{
    return this->isAdmin;
}

std::string User::getUserLogin() const
{
    return this->userLogin;
}

std::string User::getHashedPassword() const
{
    return this->hashedPassword;
}

std::string User::getEmail() const
{
    return this->email;
}

std::string User::getPhone() const
{
    return this->phone;
}

void User::setUserLogin(std::string userLogin)
{
    User::registerUserLogin(this->userLogin);
    this->userLogin = userLogin;
}

void User::setHashedPassword(std::string hashedPassword)
{
    this->hashedPassword = hashedPassword;
}

void User::setEmail(std::string email)
{
    this->email = email;
}

void User::setPhone(std::string phone)
{
    this->phone = phone;
}
