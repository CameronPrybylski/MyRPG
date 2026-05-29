#pragma once

#include <Engine/Scene/GameObject.h>
#include <map>

class Obstacle;

class RepeatPattern : public GameObject
{
public:
    RepeatPattern(glm::vec3 position, glm::vec3 scale, glm::vec3 rotation, glm::vec3 velocity, glm::vec4 color, std::string texturePath, std::string name, bool isStatic, glm::vec2 tileScale);
    ~RepeatPattern();

    void Init();

    void OnEvent(const Input& input) override;
    void Update(const Input& input, float dt) override;
    void Render(Renderer& renderer, const Camera& camera) override;
    virtual void OnCollision(std::shared_ptr<GameObject> collidedObj, glm::vec2 collisionNormal, float dt) override;

    void SetVertices(std::vector<float> vertices)
    {
        this->vertices = vertices;
        mesh->SetVertices(vertices);
    }

    void SetReverseTexturePath(std::string reverseTexturePath){this->reverseTexturePath = reverseTexturePath;}

    void AddVertices(glm::vec3 position, glm::vec3 scale, glm::vec3 rotation, glm::vec3 velocity, glm::vec4 color, std::string texturePath, std::string name, bool isStatic, glm::vec2 tileScale);

protected:
    //std::vector<std::shared_ptr<Obstacle>> patterns;
    std::vector<float> vertices;
    std::string texturePath = "";
    glm::vec2 tileScale;
    int frame = 0;
    std::string reverseTexturePath = "";
    std::map<int, std::string> texturePaths;

};