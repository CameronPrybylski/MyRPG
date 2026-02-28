#pragma once

#include <Engine/Scene/GameObject.h>

class EnemyInBattle : public GameObject {

public:
    EnemyInBattle(glm::vec3 position, glm::vec3 scale, glm::vec4 color, std::string texturePath, std::string name, int attackDamage, int xp);
    ~EnemyInBattle();
    virtual void OnEvent(const Input& input) override;
    virtual void Update(float dt) override;
    virtual void Update(const Input& input, float dt) override;
    virtual void Render(Renderer& renderer, const Camera& camera) override;
    virtual void OnCollision(std::shared_ptr<GameObject> collidedObj, glm::vec2 collisionNormal, float dt) override;

    virtual void TakeDamage(int damage);
    virtual void MakeMove();
    void SetMove(bool move)
    {
        enemyMove = move;
    }
    bool GetMove()
    {
        return enemyMove;
    }

    int GetHP()
    {
        return hp;
    }

    int GetXP()
    {
        return xp;
    }

    int GetAttackDamage()
    {
        return attackDamage;
    }

    bool IsAlive()
    {
        return isAlive;
    }

protected:
    bool isAlive;
    bool enemyMove = false;
    int hp;
    int attackDamage;
    int xp;
};