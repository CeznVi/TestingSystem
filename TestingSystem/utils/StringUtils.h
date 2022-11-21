#pragma once
#include <string>
#include <list>
#include <set>


class StringUtils
{
    public:
    static std::string fromBool(bool value);
    static bool toBool(std::string str);
    
    static std::string fromSetOfUInt(const std::set<unsigned int> & value);
    static std::set<unsigned int> toSetOfUInt(std::string str);

    static std::list<std::string> split(std::string str, const char * delimiter);

    static std::string hash(std::string str);

    static bool isValidEmail(std::string str);
    static bool isValidPhone(std::string str);
};
