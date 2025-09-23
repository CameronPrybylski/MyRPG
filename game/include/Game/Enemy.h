#pragma once

#include <Engine/Scene/GameObject.h>

class Enemy : public GameObject 
{
public:
    Enemy(glm::vec3 position, glm::vec3 scale, glm::vec3 velocity, glm::vec4 color, std::string texturePath, std::string name, bool isStatic);
    ~Enemy();
    void OnEvent(const Input& input) override;
    void Update(float dt) override;
    void Update(const Input& input, float dt) override;
    void Render(Renderer& renderer, const Camera& camera) override;
    void OnCollision(std::shared_ptr<GameObject> collidedObj, glm::vec2 collisionNormal, float dt) override;

    bool CheckIsAlive(){return isAlive;}

private:
    bool isAlive;
    glm::vec3 velocity;
};