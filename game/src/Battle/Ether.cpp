#include <Game/Ether.h>

Ether::Ether()
{
    this->name = "Ether";
}

std::pair<std::string, int> Ether::UseItem()
{
   return std::make_pair("mp", 5);
}