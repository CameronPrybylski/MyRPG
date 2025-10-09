#pragma once

class Weapon {
    
public: 
    Weapon(int damage);
    ~Weapon();

    int GetDamage()
    {
        return damage;
    }

private:
    int damage;
};