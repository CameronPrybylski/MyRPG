#pragma once

#include <Engine/Scene/GameObject.h>

class Obstacle;

class RepeatPattern : public GameObject
{
public:
    RepeatPattern(glm::vec3 position, glm::vec3 scale, glm::vec3 rotation, glm::vec3 velocity, glm::vec4 color, std::string texturePath, std::string name, bool isStatic);
    ~RepeatPattern();

    void Init();

    void OnEvent(const Input& input) override;
    void Update(const Input& input, float dt) override;
    void Render(Renderer& renderer, const Camera& camera) override;
    void OnCollision(std::shared_ptr<GameObject> collidedObj, glm::vec2 collisionNormal, float dt) override;

private:
    //std::vector<std::shared_ptr<Obstacle>> patterns;
    //std::vector<
    std::string texturePath = "";

};