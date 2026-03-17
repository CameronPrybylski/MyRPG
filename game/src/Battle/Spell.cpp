#include <Game/Spell.h>

Spell::Spell(int damage, int mpCost)
{
    this->damage = damage;
    this->mpCost = mpCost;
}

Spell::~Spell()
{
}