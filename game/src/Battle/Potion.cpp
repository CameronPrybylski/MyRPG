#include <Game/Potion.h>

Potion::Potion()
{
    this->name = "Potion";
}

std::pair<std::string, int> Potion::UseItem()
{
   return std::make_pair("hp", 5);
}