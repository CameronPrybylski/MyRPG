#pragma once

#include <Engine/Scene/GameObject.h>

class Sword : public GameObject
{
public:
    Sword(glm::vec3 position, glm::vec3 scale, glm::vec3 velocity, glm::vec4 color, std::string texturePath, std::string name, bool isStatic);
    ~Sword();
    void OnEvent(const Input& input) override;
    //void Update(float dt) override;
    void Update(const Input& input, float dt) override;
    void Render(Renderer& renderer, const Camera& camera) override;
    void OnCollision(std::shared_ptr<GameObject> collidedObj, glm::vec2 collisionNormal, float dt) override;

    bool usingSword;
    float length;
    float width;
   // std::shared_ptr<Player> player;

};