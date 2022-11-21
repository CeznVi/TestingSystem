#include <iostream>
#include <string>
#include "Logger.h"

// #define LOG_ENABLED 1

void Logger::log(std::string location, std::string message)
{
    #ifdef LOG_ENABLED
    std::cout << "[Location:" << location << "]: " << message << std::endl;
    #endif
}
