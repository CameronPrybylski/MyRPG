#pragma once

#include <Engine/Scene/GameObject.h>

class EnemyInBattle : public GameObject {

public:
    EnemyInBattle(glm::vec3 position, glm::vec3 scale, glm::vec4 color, std::string texturePath, std::string name, int attackDamage);
    ~EnemyInBattle();
    void OnEvent(const Input& input) override;
    void Update(float dt) override;
    void Update(const Input& input, float dt) override;
    void Render(Renderer& renderer, const Camera& camera) override;
    void OnCollision(std::shared_ptr<GameObject> collidedObj, glm::vec2 collisionNormal, float dt) override;

    void TakeDamage(int damage);
    void MakeMove();
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

    int GetAttackDamage()
    {
        return attackDamage;
    }

    bool IsAlive()
    {
        return isAlive;
    }

private:
    bool isAlive;
    bool enemyMove = false;
    int hp;
    int attackDamage;
};