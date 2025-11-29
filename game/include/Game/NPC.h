#pragma once

#include <Engine/Scene/GameObject.h>

class NPC : public GameObject {

public:
    NPC(glm::vec3 position, glm::vec3 scale, glm::vec3 velocity, glm::vec4 color, std::string texturePath, std::string name, bool isStatic, std::vector<std::string> dialogue);
    ~NPC();
    void OnEvent(const Input& input) override;
    void Update(const Input& input, float dt) override;
    void Render(Renderer& renderer, const Camera& camera) override;
    void OnCollision(std::shared_ptr<GameObject> collidedObj, glm::vec2 collisionNormal, float dt) override;
    
    void Dialogue();
    bool GetTalking(){return talking;}
    void SetTalking(bool talking){this->talking = talking;}
    std::vector<std::string> GetDialogue(){return dialogue;}

private:
    std::vector<std::string> dialogue;
    bool talking = false;

};