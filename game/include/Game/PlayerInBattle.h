#pragma once
#include <Engine/Scene/GameObject.h>
#include <Game/Sword.h>
#include <Game/Weapon.h>

class ConsumableItem;
class Spell;

class PlayerInBattle : public GameObject {

public:
    PlayerInBattle(glm::vec3 position, glm::vec3 scale, glm::vec4 color, std::string texturePath, std::string name, bool isStatic);
    ~PlayerInBattle();

    void OnEvent(const Input& input) override;
    void Update(const Input& input, float dt) override;
    void Render(Renderer& renderer, const Camera& camera) override;
    void OnCollision(std::shared_ptr<GameObject> collidedObj, glm::vec2 collisionNormal, float dt) override;

    void Hit(glm::vec2 collisionNormal, float dt);
    void AddItem(std::string name, std::shared_ptr<GameObject> item);
    void AddConsumableItem(std::string name, std::shared_ptr<ConsumableItem> item);
    void AddWeapon(std::string name, std::shared_ptr<Weapon> weapon);
    void AddSpell(std::string name, std::shared_ptr<Spell> spell);
    void PositionSword();

    void SetMove(std::string move)
    {
        playerMove = move;
    }

    std::string GetMove(){return playerMove;}

    void SetHP(int hp)
    {
        this->hp = hp;
    }

    void SetMaxHP(int maxHP)
    {
        this->maxHP = maxHP;
    }

    int GetHP()
    {
        return hp;
    }

    int GetMaxHP()
    {
        return maxHP;
    }

    void SetMP(int mp)
    {
        this->mp = mp;
    }

    void SetMaxMP(int maxMP)
    {
        this->maxMP = maxMP;
    }

    int GetMP()
    {
        return mp;
    }

    int GetMaxMP()
    {
        return maxMP;
    }

    int GetAttackDamage()
    {
        return strength * equippedWeapon->GetDamage();
    }

    int GetMagicDamage(std::string magicType);

    void TakeDamage(int damage)
    {
        hp -= damage;
    }

    int GetLevel(){return level;}
    int GetXP(){return xp;}
    int GetXPNeeded(){return xpNeeded;}
    int GetStrength(){return strength;}

    void SetLevel(int level){this->level = level;}
    void SetXP(int xp){this->xp = xp;}
    void SetXPNeeded(int xpNeeded){this->xpNeeded = xpNeeded;}
    void SetStrength(int strength){this->strength = strength;}

    void AddToXP(int addedXP){xp += addedXP;}
    void CheckXP();

    void ChangeWeapon(std::string weaponName);
    std::shared_ptr<Weapon> GetEquippedWeapon()
    {
        return equippedWeapon;
    }

    std::unordered_map<std::string, std::shared_ptr<Weapon>> GetWeapons()
    {
        return weapons;
    }

    bool hit = false;

    bool usingSword = false;
    bool stop = false;
    bool inBattle = false;
    std::string positionFacing = "Down";

    std::string playerMove = "";

    float timeSinceHit = 0.0f;

    std::unordered_map<std::string, std::vector<std::shared_ptr<GameObject>>> items;
    void UseItem(std::string playerMove);
    int ConsumableItemCount(std::string item)
    {
        return consumableItems[item].size();
    }

    std::unordered_map<std::string, std::vector<std::shared_ptr<ConsumableItem>>> GetConsumableItems(){return this->consumableItems;}

    int GetGil(){return this->gil;}
    void SetGil(int gil){this->gil = gil;}

private:
    int hp;
    int maxHP;
    bool alive = true;
    std::unordered_map<std::string, std::shared_ptr<Weapon>> weapons;
    std::unordered_map<std::string, std::shared_ptr<Spell>> spells;
    std::unordered_map<std::string, std::vector<std::shared_ptr<ConsumableItem>>> consumableItems;
    std::shared_ptr<Weapon> equippedWeapon;
    
    int level;
    int strength;
    int magicLevel;
    int mp;
    int maxMP;
    int xp;
    int xpNeeded;
    int xpIncrement;

    int gil;


};