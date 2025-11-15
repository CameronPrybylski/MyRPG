#pragma once
#include <Engine/Scene/GameObject.h>
#include <Game/Sword.h>
#include <Game/Weapon.h>

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
    void AddWeapon(std::string name, std::shared_ptr<Weapon> weapon);
    void PositionSword();

    void SetMove(std::string move)
    {
        playerMove = move;
    }

    void SetHP(int hp)
    {
        this->hp = hp;
    }

    int GetHP()
    {
        return hp;
    }

    int GetAttackDamage()
    {
        return strength * equippedWeapon->GetDamage();
    }

    void TakeDamage(int damage)
    {
        hp -= damage;
    }

    int GetLevel(){return level;}
    int GetXP(){return xp;}
    int GetStrength(){return strength;}

    void SetLevel(int level){this->level = level;}
    void SetXP(int xp){this->xp = xp;}
    void SetStrength(int strength){this->strength = strength;}

    bool hit = false;

    bool usingSword = false;
    bool stop = false;
    bool inBattle = false;
    std::string positionFacing = "Down";

    std::string playerMove = "";

    float timeSinceHit = 0.0f;

    std::unordered_map<std::string, std::shared_ptr<GameObject>> items;
private:
    int hp;
    bool alive = true;
    std::unordered_map<std::string, std::shared_ptr<Weapon>> weapons;
    std::shared_ptr<Weapon> equippedWeapon;
    
    int level;
    int strength;
    int xp;


};