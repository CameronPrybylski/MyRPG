#pragma once

#include <Game/ConsumableItem.h>

class Potion : public ConsumableItem
{
public:
    Potion();

    std::pair<std::string, int> UseItem() override;
};