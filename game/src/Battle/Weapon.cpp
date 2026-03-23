#include <Game/Weapon.h>

Weapon::Weapon(int damage, std::string name)
{
    this->damage = damage;
    this->name = name;
}

Weapon::~Weapon()
{
}