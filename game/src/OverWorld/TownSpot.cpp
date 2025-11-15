#include <Game/TownSpot.h>

TownSpot::TownSpot(glm::vec3 position, glm::vec3 scale, glm::vec4 color, std::string texturePath, std::string name)
{
    mesh = AssetManager::GetMesh("quadMesh");
    transform.position = position;
    transform.scale = scale;
    rigidBody.isStatic = true;
    this->color = color;
    this->name = name;
    enterTown = false;
    if(texturePath != ""){
        shaderName = "textureShader";
        texture.Create(texturePath);
    }else{
        shaderName = "objectShader";
    }
}

TownSpot::~TownSpot()
{
}

void TownSpot::OnEvent(const Input &input)
{
}

void TownSpot::Update(const Input &input, float dt)
{
}

void TownSpot::Render(Renderer &renderer, const Camera &camera)
{
    if(shaderName == "textureShader"){
        renderer.DrawTexturedQuad(*mesh, transform, camera, AssetManager::GetShader(shaderName), texture, color);
    }else{
        renderer.DrawQuad(*mesh, transform, camera, AssetManager::GetShader(shaderName), color);
    }
}

void TownSpot::OnCollision(std::shared_ptr<GameObject> collidedObj, glm::vec2 collisionNormal, float dt)
{
    if(collidedObj->name == "player")
    {
        SetEnterTown(true);
    }
}
