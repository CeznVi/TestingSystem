#include <string>
#include "entities.h"


not_unique_id::not_unique_id(const std::string& entity)
{
    this->entity = std::string(entity);
}

const char* not_unique_id::what() const noexcept
{
    return ("Receiving not unique ID on \"" + this->entity + "\" entity.").c_str();
}

const char* not_unique_user_login::what() const noexcept
{
    return "Given userLogin is already exist";
}

assembly_entity_error::assembly_entity_error(const std::string& entity, const std::string& message)
{
    this->entity = std::string(entity);
    this->message = std::string(message);
}

const char* assembly_entity_error::what() const noexcept
{
    return ("Error while assembly \"" + this->entity + "\" entity. \"" + this->message + "\"").c_str();
}
