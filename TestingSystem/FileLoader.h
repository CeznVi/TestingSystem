#pragma once
#include <string>
#include <memory>
#include "entities/entities.h"
#include <exception>


class file_loader_error : public std::exception { };

class file_not_exist : public file_loader_error
{
public:
    file_not_exist(const std::string& filePath);
    const char* what() const noexcept override;

private:
    std::string filePath;
};

class invalid_path : public file_loader_error
{
public:
    invalid_path(const std::string& filePath);
    const char* what() const noexcept override;

private:
    std::string filePath;
};

class FileLoader
{
public:
    static EntitiesBatch readFile(std::string filePath);
    static void writeFile(std::string filePath, const EntitiesBatch& eb);
    static void writeTestReportToFile(std::string filePath, std::shared_ptr<TestReport> testReport);
};

