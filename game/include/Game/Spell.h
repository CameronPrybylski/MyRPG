#pragma once

class Spell {
    
public: 
    Spell(int damage, int mpCost);
    ~Spell();

    int GetDamage()
    {
        return damage;
    }
    int GetMPCost()
    {
        return mpCost;
    }

private:
    int damage;
    int mpCost;
};