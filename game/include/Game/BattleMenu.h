#pragma once
#include <Engine/Scene/GameObject.h>
#include <Engine/Renderer/TextTexture.h>

class BattleMenu : public GameObject {

public:

    BattleMenu(glm::vec3 position, glm::vec3 scale, glm::vec4 color, std::string texturePath, std::string name);
    ~BattleMenu();

    void OnEvent(const Input& input) override;
    void Update(const Input& input, float dt) override;
    void Render(Renderer& renderer, const Camera& camera) override;
    void OnCollision(std::shared_ptr<GameObject> collidedObj, glm::vec2 collisionNormal, float dt) override;


private:

    std::unordered_map<std::string, std::shared_ptr<GameObject>> menuItems;
    TextTexture text;

};