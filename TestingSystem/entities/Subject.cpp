#include <string>
#include "entities.h"
#include "../utils/Logger.h"


Subject::Subject(std::string name)
{
    this->name = name;
}

Subject::~Subject()
{
    Logger::log("Subject(id="+this->name+")", "Destructor");
}

std::string Subject::toString() noexcept
{
    return this->name;
}

std::string Subject::renderInList() noexcept
{
    return this->name;
}

std::string Subject::getName()
{
    return this->name;
}
