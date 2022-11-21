#pragma once
#include <string>
#include "scenes/scenes.h"


class UIController
{
public:
    void renderScene(std::unique_ptr<BaseScene>& scene);
    std::string getInput();
};
