#pragma once

#include <Game/ConsumableItem.h>

class Ether : public ConsumableItem
{
public:
    Ether();

    std::pair<std::string, int> UseItem() override;
};