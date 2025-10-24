#include <Game/BattleMenu.h>
#include <Game/MenuItem.h>

BattleMenu::BattleMenu(glm::vec3 position, glm::vec3 scale, glm::vec4 color, std::string texturePath, std::string name) : Menu(position, scale, color, texturePath, name)
{
    
}

BattleMenu::~BattleMenu()
{
}

void BattleMenu::OnEvent(const Input &input)
{
    if(input.IsKeyDown("W") && cursor->transform.position.y < cursorMaxHeight)
    {
        cursor->transform.position.y += 20.0f;
    }
    if(input.IsKeyDown("S") && cursor->transform.position.y > cursorMinHeight)
    {
        cursor->transform.position.y -= 20.0f;
    }
    if(input.IsKeyDown("RETURN"))
    {
        if(menuName == "MenuItems")
        {
            for(auto menuItem : menuItems)
            {
                if(cursor->transform.position.y == menuItem.second->transform.position.y)
                {
                    menuName = menuItem.second->GetText() + "MenuItems";
                }
            }
        }
        else if(menuName == "AttackMenuItems")
        {
            for(auto attackMenuItem : attackMenuItems)
            {
                if(cursor->transform.position.y == attackMenuItem.second->transform.position.y)
                {
                    if(attackMenuItem.second->GetText() != "Back")
                    {
                        playerMove = "Attack" + attackMenuItem.second->GetText();
                    }
                    menuName = "MenuItems";
                }
            }
        }
    }

}

void BattleMenu::Update(const Input &input, float dt)
{
    //std::cout << cursor->transform.position.y << std::endl;
}

void BattleMenu::Render(Renderer &renderer, const Camera &camera)
{
    renderer.DrawQuad(*mesh, transform, camera, AssetManager::GetShader(shaderName), color);
    if(menuName == "MenuItems")
    {
        for(auto menuItem : menuItems)
        {
            menuItem.second->Render(renderer, camera);
        }
    }
    else if(menuName == "AttackMenuItems")
    {
        for(auto attackMenuItem : attackMenuItems)
        {
            attackMenuItem.second->Render(renderer, camera);
        }
    }
    else
    {
        menuName = "MenuItems";
        for(auto menuItem : menuItems)
        {
            menuItem.second->Render(renderer, camera);
        }
    }
    renderer.DrawQuad(*cursor->mesh, cursor->transform, camera, AssetManager::GetShader(cursor->shaderName), cursor->color);
}

void BattleMenu::OnCollision(std::shared_ptr<GameObject> collidedObj, glm::vec2 collisionNormal, float dt)
{
}

void BattleMenu::AddAttackMenuItem(std::string name, glm::vec3 position, glm::vec3 scale, glm::vec4 color, std::string fontPath, std::string text)
{
    std::shared_ptr<MenuItem> menuItem = std::make_shared<MenuItem>(name, position, scale, color, fontPath, text);
    attackMenuItems[text] = menuItem;
}

void BattleMenu::SetDeadEnemies(std::vector<std::string> deadEnemies)
{
    for(std::string enemyName : deadEnemies)
    {
        attackMenuItems.erase(enemyName);
    }
}

void BattleMenu::UpdatePlayerHP(int hp)
{
    menuItems["menuItemPlayerHP"]->ChangeText("HP: " + std::to_string(hp));
}