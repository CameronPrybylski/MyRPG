#include <Game/Corridor.h>

Corridor::Corridor(glm::vec3 position, glm::vec3 scale, glm::vec3 rotation, glm::vec3 velocity, glm::vec4 color, std::string texturePath, std::string name, bool isStatic, std::set<int> gaps) : 
    Obstacle(position, scale,  rotation,  velocity, color, texturePath, name, isStatic)
{
    mesh = AssetManager::GetMesh("quadMesh");
    transform.position = position;
    transform.scale = scale;
    transform.rotation = rotation;
    rigidBody.velocity = velocity;
    rigidBody.isStatic = isStatic;
    this->color = color;
    this->name = name;
    this->velocity = velocity;
    this->gaps = gaps;
    if(texturePath != ""){
        shaderName = "textureShader";
        texture.Create(texturePath);
    }else{
        shaderName = "objectShader";
    }
    SetWalls();
}

Corridor::~Corridor()
{
}

void Corridor::SetWalls()
{
    walls.clear();
    bool secondHalf = false;
    glm::vec3 leftRightPos{(transform.scale.x / 2) + 10.0f, 0.0f, 0.0f};
    glm::vec3 upDownPos{0.0f, (transform.scale.y / 2), 0.0f};

    glm::vec3 borderPos;
    glm::vec3 borderScale;
    glm::vec4 borderColor = glm::vec4(1.0f, 1.0f, 1.0f, 0.5f);
    for(int i = 0; i < 4; i++)
    {
        std::string borderName = "border" + std::to_string(i);
        if(gaps.count(i))
        {
            continue;
        }
        if(i > 1 && !secondHalf)
        {
            leftRightPos *= -1;
            upDownPos    *= -1;
            secondHalf = true;
        }
        if(i % 2 == 0)
        {
            borderPos = transform.position + leftRightPos;
            borderScale = glm::vec3(20.0f, transform.scale.y, 0.0f);
        }
        else
        {
            borderPos = transform.position + upDownPos;
            borderScale = glm::vec3(transform.scale.x, 20.0f, 0.0f);
        }
        borderPos += glm::vec3{0.0f, 0.0f, 1.0f};
        std::shared_ptr<Obstacle> border = std::make_shared<Obstacle>(borderPos, borderScale, glm::vec3(0.0f), glm::vec3(0.0f), borderColor, "", borderName, true);
        walls.push_back(border);
    }
}