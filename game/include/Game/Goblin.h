#pragma once

#include <Game/EnemyInBattle.h>


class Goblin : public EnemyInBattle 
{

public:
    Goblin(glm::vec3 position, glm::vec3 scale, glm::vec4 color, std::string texturePath, std::string name, int attackDamage, int xp);
    ~Goblin();
    virtual void OnEvent(const Input& input) override;
    virtual void Update(float dt) override;
    virtual void Update(const Input& input, float dt) override;
    virtual void Render(Renderer& renderer, const Camera& camera) override;
    virtual void OnCollision(std::shared_ptr<GameObject> collidedObj, glm::vec2 collisionNormal, float dt) override;

    virtual void TakeDamage(int damage) override;
    virtual void MakeMove() override;
};