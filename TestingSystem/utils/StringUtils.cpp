#include <string>
#include <list>
#include <set>
#include <algorithm>
#include <regex>
#include "md5.h"
#include "StringUtils.h"


std::string StringUtils::fromBool(bool value)
{
    if (value) return "true";
    else return "false";
}

bool StringUtils::toBool(std::string str)
{
    if (str == "true") return true;
    else return false;
}

std::string StringUtils::fromSetOfUInt(const std::set<unsigned int> & value)
{
    std::string result = "";

    for(auto it = value.begin(); it != value.end(); ++it)
    {
        result = result + std::to_string(*it) + ";";
    }

    return result;
}

std::set<unsigned int> StringUtils::toSetOfUInt(std::string str)
{
    std::set<unsigned int> result;
    auto tmp = StringUtils::split(str, ";");
    for (auto it = tmp.begin(); it != tmp.end(); ++it)
    {
        result.insert(std::stoi(*it));
    }

    return result;
}

std::list<std::string> StringUtils::split(std::string str, const char * delimiter)
{
    std::list<std::string> result;

    std::string tmp = "";
    for (auto it = str.begin(); it != str.end(); ++it)
    {
        if (*it == delimiter[0])
        {
            result.push_back(tmp);
            tmp = "";
        }
        else
        {
            tmp = tmp + *it;
        }
    }
    if (tmp.length() > 0) {
        result.push_back(tmp);
    }

    return result;
}

std::string StringUtils::hash(std::string str)
{
    return md5(str);
}

bool StringUtils::isValidEmail(std::string str)
{
    return std::regex_match(str, std::regex("(\\w+)(\\.|_)?(\\w*)@(\\w+)(\\.(\\w+))+"));
}

bool StringUtils::isValidPhone(std::string str)
{
    return std::regex_match(str, std::regex("^[\\+]?[(]?[0-9]{3}[)]?[-\\s\\.]?[0-9]{3}[-\\s\\.]?[0-9]{4,6}$"));
}
