#include <string>
#include "scenes.h"


const std::list<std::string>* const BaseScene::getView() noexcept
{
    return &this->view;
}

void BaseScene::setWarning()
{
    if (!this->warning)
    {
        this->warning = true;
        this->view.push_front("Ви ввели недопустиме значення. Спробуйте ще\n");
    }
}
