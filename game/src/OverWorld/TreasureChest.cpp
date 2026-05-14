#include <Game/TreasureChest.h>

TreasureChest::TreasureChest(glm::vec3 position, glm::vec3 scale, glm::vec4 color, std::string texturePath, std::string name)
{
    mesh = AssetManager::GetMesh("quadMesh");
    transform.position = position;
    transform.scale = scale;
    rigidBody.isStatic = true;
    this->color = color;
    this->name = name;
    openChest = false;
    this->empty = false;
    this->texturePath = texturePath;
    if(texturePath != ""){
        shaderName = "textureShader";
        texture.Create(texturePath);
    }else{
        shaderName = "objectShader";
    }
}

TreasureChest::~TreasureChest()
{
}

void TreasureChest::OnEvent(const Input &input)
{
    if(input.IsKeyDown("SPACE"))
    {
        touchChest = true;
    }
    else
    {
        touchChest = false;
    }
}

void TreasureChest::Update(const Input &input, float dt)
{
}

void TreasureChest::Render(Renderer &renderer, const Camera &camera)
{
    if(shaderName == "textureShader"){
        renderer.DrawTexturedQuad(*mesh, transform, camera, AssetManager::GetShader(shaderName), texture, color);
    }else{
        renderer.DrawQuad(*mesh, transform, camera, AssetManager::GetShader(shaderName), color);
    }
}

void TreasureChest::OnCollision(std::shared_ptr<GameObject> collidedObj, glm::vec2 collisionNormal, float dt)
{
    if(collidedObj->name == "player" && touchChest && !empty)
    {
        SetOpenChest(true);
    }
}
