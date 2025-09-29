#include <Game/BattleMenu.h>
#include <Engine/Renderer/TextTexture.h>

BattleMenu::BattleMenu(glm::vec3 position, glm::vec3 scale, glm::vec4 color, std::string texturePath, std::string name)
{
    transform.position = position;
    transform.scale = scale;
    this->color = color;
    this->name = name;

    mesh = AssetManager::GetMesh("quadMesh");

    if(texturePath != ""){
        shaderName = "textureShader";
        texture.Create(texturePath);
    }else{
        shaderName = "objectShader";
    }

    for(int i = 0; i < 4; i++)
    {
        std::shared_ptr<GameObject> menuItem = std::make_shared<GameObject>();
        menuItem->mesh = AssetManager::GetMesh("quadMesh");
        menuItems["menuItem" + std::to_string(i)] = menuItem;
        menuItem->transform.position = {100.0f,(i * 40.0f) + 15.0f, 0.0f};
        menuItem->transform.scale = {100.0f, 50.0f, 0.0f};
        menuItem->color = {1.0f, 1.0f, 1.0f, 1.0f};
        text.Create("/Users/cameronprzybylski/Documents/C++/C++ Projects/MyRPG/fonts/OpenSans-Bold.ttf", "Hello");

    }
}

BattleMenu::~BattleMenu()
{
}

void BattleMenu::OnEvent(const Input &input)
{
    if(input.IsKeyDown("W"))
    {

    }
}

void BattleMenu::Update(const Input &input, float dt)
{
}

void BattleMenu::Render(Renderer &renderer, const Camera &camera)
{
    renderer.DrawQuad(*mesh, transform, camera, AssetManager::GetShader(shaderName), color);
    for(auto menuItem : menuItems)
    {
        //renderer.DrawQuad(*menuItem.second->mesh, menuItem.second->transform, camera, AssetManager::GetShader(shaderName), menuItem.second->color);
        //renderer.DrawTexturedQuad(*menuItem.second->mesh, menuItem.second->transform, camera, AssetManager::GetShader(shaderName), menuItem.second->color);
        renderer.DrawTexturedQuad(*menuItem.second->mesh, menuItem.second->transform, camera, AssetManager::GetShader("textureShader"), text, menuItem.second->color);
    }
}

void BattleMenu::OnCollision(std::shared_ptr<GameObject> collidedObj, glm::vec2 collisionNormal, float dt)
{
}
