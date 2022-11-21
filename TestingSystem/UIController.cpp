#include <iostream>
#include "UIController.h"
#include "scenes/scenes.h"


void UIController::renderScene(std::unique_ptr<BaseScene>& scene)
{
    system("cls");
    auto view = scene->getView();
    for (auto it = view->begin(); it != view->end(); ++it)
    {
        std::cout << *it;
    }
}

std::string UIController::getInput()
{
    std::string input;
    std::getline(std::cin, input);
    return input;
}
