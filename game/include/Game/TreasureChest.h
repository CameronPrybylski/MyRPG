#pragma once

#include<Engine/Scene/GameObject.h>

class TreasureChest : public GameObject
{

public:
    TreasureChest(glm::vec3 position, glm::vec3 scale, glm::vec4 color, std::string texturePath, std::string name);
    ~TreasureChest();

    void OnEvent(const Input& input) override;
    void Update(const Input& input, float dt) override;
    void Render(Renderer& renderer, const Camera& camera) override;
    void OnCollision(std::shared_ptr<GameObject> collidedObj, glm::vec2 collisionNormal, float dt) override;

    void SetOpenChest(bool openChest)
    {
        this->openChest = openChest;
    }

    bool GetOpenChest(){return this->openChest;}

    void SetContents(std::unordered_map<std::string, int> contents)
    {
        this->contents = contents;
    }

    std::unordered_map<std::string, int> GetContents()
    {
        return this->contents;
    }

    bool Empty(){return this->empty;}
    void SetEmpty(bool empty){this->empty = empty;}


private:
    bool openChest;
    bool touchChest;
    bool empty;
    std::unordered_map<std::string, int> contents;

};