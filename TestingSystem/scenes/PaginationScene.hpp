#pragma once
#include <string>
#include <list>
#include <memory>
#include "scenes.h"
#include <stdexcept>
#include "../entities/entities.h"
#include "../utils/Logger.h"



template <class EntityScene, class StepBackScene, class E>
PaginationScene<EntityScene, StepBackScene, E>::PaginationScene(std::list<std::shared_ptr<E>> entityList)
{
    this->warning = false;
    this->entityList = entityList;
    this->currentPage = 0;
    this->totalPages = (this->entityList.size() + MAX_PER_PAGE - 1) / MAX_PER_PAGE;

    this->calculateView();
}

template <class EntityScene, class StepBackScene, class E>
PaginationScene<EntityScene, StepBackScene, E>::~PaginationScene()
{
    Logger::log("PaginationScene", "Destructor");
}


template <class EntityScene, class StepBackScene, class E>
BaseScene* PaginationScene<EntityScene, StepBackScene, E>::handleInput(StateManager& stateManager, const std::string& input) noexcept
{
    if (input == "b")
    {
        return new StepBackScene();
    }

    else if (input == "n" && (this->currentPage+1) < this->totalPages)
    {
        this->currentPage++;
        this->calculateView();
        return this;
    }
    else if (input == "p" && this->currentPage > 0)
    {
        this->currentPage--;
        this->calculateView();
        return this;
    }
    
    try
    {
        auto i = std::stoi(input);
        if (i >= 0 && i <= 9)
        {
            auto it = this->entityList.begin();
            auto index = this->currentPage * MAX_PER_PAGE + i;
            if (index < this->entityList.size())
            {
                std::advance(it, index);
                return this->processEnitity(*it, stateManager);
            }  
        }
    }
    catch (const std::invalid_argument& e) { }

    this->setWarning();
    return this;
}

template <class EntityScene, class StepBackScene, class E>
void PaginationScene<EntityScene, StepBackScene, E>::calculateView()
{
    this->view.assign({ "Оберіть з наступного списку:\n" });
    this->warning = false;
        
    if (this->totalPages > 1)
        this->view.push_front("Сторінка " + std::to_string(this->currentPage+1) + "/" + std::to_string(this->totalPages) + "\n");

    auto i = 0;
    auto it = this->entityList.begin();
    auto end = this->entityList.end();
    std::advance(it, this->currentPage * MAX_PER_PAGE);
    for (; it != end && i < MAX_PER_PAGE; ++it, ++i)
    {
        this->view.push_back(std::to_string(i) + ") " + (*it)->renderInList() + "\n");
    }
    if (this->currentPage > 0)
    {
        this->view.push_back( "Введіть 'p', щоб переглянути попереднью сторінку\n" );
    }
    if (i == MAX_PER_PAGE && it != end)
    {
        this->view.push_back("Введіть 'n', щоб переглянути наступку сторінку\n" );
    }
    this->view.push_back("Введіть 'b', щоб повернутися назад\n" );
}

template <class EntityScene, class StepBackScene, class E>
BaseScene* PaginationScene<EntityScene, StepBackScene, E>::processEnitity(std::shared_ptr<E> entity, StateManager& stateManager)
{
    return new EntityScene(entity);
}
