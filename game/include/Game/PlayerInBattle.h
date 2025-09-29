#pragma once
#include <Engine/Scene/GameObject.h>
#include <Game/Sword.h>

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
    void PositionSword();

    bool hit = false;
    bool alive = true;
    bool usingSword = false;
    bool stop = false;
    bool inBattle = false;
    std::string positionFacing = "Down";

    int hp;
    float timeSinceHit = 0.0f;

    std::unordered_map<std::string, std::shared_ptr<GameObject>> items;

};