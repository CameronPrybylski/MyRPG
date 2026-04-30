#pragma once

#include <Engine/Scene/GameObject.h>

class NPC : public GameObject {

public:
    NPC(glm::vec3 position, glm::vec3 scale, glm::vec3 velocity, glm::vec4 color, std::string texturePath, std::string name, bool isStatic, std::vector<std::string> dialogue, float maxDist);
    ~NPC();
    void OnEvent(const Input& input) override;
    void Update(const Input& input, float dt) override;
    void Render(Renderer& renderer, const Camera& camera) override;
    void OnCollision(std::shared_ptr<GameObject> collidedObj, glm::vec2 collisionNormal, float dt) override;
    
    void Dialogue();
    bool GetTalking(){return talking;}
    void SetTalking(bool talking){this->talking = talking;}
    std::vector<std::string> GetDialogue(){return dialogue;}

    void SetDialogueTree(std::vector<std::unordered_map<std::string, std::vector<std::string>>> dialogueTree){this->dialogueTree = dialogueTree;}
    std::vector<std::unordered_map<std::string, std::vector<std::string>>> GetDialogueTree(){return this->dialogueTree;}
    bool HasDialogueTree(){return !dialogueTree.empty();}

    void SetTexturePath2(std::string texturePath2){this->texturePath2 = texturePath2;}


private:
    std::vector<std::string> dialogue;
    bool talking = false;
    glm::vec3 velocity;
    float maxDist;
    float distance = 0;
    std::string texturePath = "";
    std::string texturePath2 = "";
    float frame = 1.0f;
    float totalVelocity = 0.0f;
    std::vector<std::unordered_map<std::string, std::vector<std::string>>> dialogueTree;

};