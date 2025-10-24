#include <Game/SaveSpot.h>

SaveSpot::SaveSpot(glm::vec3 position, glm::vec3 scale, glm::vec4 color, std::string texturePath, std::string name)
{
    mesh = AssetManager::GetMesh("quadMesh");
    transform.position = position;
    transform.scale = scale;
    rigidBody.isStatic = true;
    this->color = color;
    this->name = name;
    saveGame = false;
    if(texturePath != ""){
        shaderName = "textureShader";
        texture.Create(texturePath);
    }else{
        shaderName = "objectShader";
    }
}

SaveSpot::~SaveSpot()
{
}

void SaveSpot::OnEvent(const Input &input)
{
}

void SaveSpot::Update(const Input &input, float dt)
{
}

void SaveSpot::Render(Renderer &renderer, const Camera &camera)
{
    if(shaderName == "textureShader"){
        renderer.DrawTexturedQuad(*mesh, transform, camera, AssetManager::GetShader(shaderName), texture, color);
    }else{
        renderer.DrawQuad(*mesh, transform, camera, AssetManager::GetShader(shaderName), color);
    }
}

void SaveSpot::OnCollision(std::shared_ptr<GameObject> collidedObj, glm::vec2 collisionNormal, float dt)
{
    if(collidedObj->name == "player")
    {
        SetSaveGame(true);
    }
}
