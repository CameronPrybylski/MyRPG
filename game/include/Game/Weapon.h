#pragma once

#include <string>

class Weapon {
    
public: 
    Weapon(int damage, std::string name);
    ~Weapon();

    int GetDamage()
    {
        return damage;
    }

    std::string GetName()
    {
        return name;
    }

private:
    int damage;
    std::string name;
};