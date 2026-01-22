#pragma once

#include <Engine/Scene/GameObject.h>

#include <utility>

class ConsumableItem : public GameObject
{
public:
    virtual std::pair<std::string, int> UseItem() = 0;

protected:
    std::string name;

private:

};