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
    if(input.IsKeyDown("W") || input.IsKeyDown("S"))
    {
        MoveCursor(input);
    }
    if(input.IsKeyDown("RETURN"))
    {
        if(menuName == "MenuItems")
        {
            for(auto menuItem : menuItems)
            {
                if(cursor->transform.position.y == menuItem.second->transform.position.y && std::abs(cursor->transform.position.x - menuItem.second->transform.position.x) <= 100.0f )
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
        else if(menuName == "MagicMenuItems")
        {
            for(auto magicMenuItem : magicMenuItems)
            {
                if(cursor->transform.position.y == magicMenuItem.second->transform.position.y)
                {
                    if(magicMenuItem.second->GetText() != "Back" && magicMenuItem.second->GetText().find("Cur") == std::string::npos)
                    {
                        magicMove = "Magic" + magicMenuItem.second->GetText();
                        menuName = "AttackMenuItems";
                        break;
                    }
                    else
                    {
                        magicMove = "Magic" + magicMenuItem.second->GetText();
                        playerMove = magicMove;
                    }
                    menuName = "MenuItems";
                }
            }
        }
        else if(menuName == "ItemsMenuItems")
        {
            for(auto itemMenuItem : itemMenuItems)
            {
                if(cursor->transform.position.y == itemMenuItem.second->transform.position.y)
                {
                    if(itemMenuItem.second->GetText() != "Back" )
                    {
                        playerMove = "UseItem" + itemMenuItem.second->GetText();
                        itemCount[itemMenuItem.second->GetText()]--;
                    }
                    menuName = "MenuItems";
                }
            }
        }
    }

}

void BattleMenu::Update(const Input &input, float dt)
{
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
    else if(menuName == "MagicMenuItems")
    {
        for(auto magicMenuItem : magicMenuItems)
        {
            magicMenuItem.second->Render(renderer, camera);
        }
    }
    else if(menuName == "ItemsMenuItems")
    {
        for(auto itemMenuItem : itemMenuItems)
        {
            itemMenuItem.second->Render(renderer, camera);
            if(itemMenuItem.first.find("Back") == std::string::npos)
            {
                std::unique_ptr<MenuItem> itemCountMenuItem = std::make_unique<MenuItem>("ItemCount", itemMenuItem.second->GetEndPosition() + glm::vec3(25.0f, 0.0f, 0.0f), itemMenuItem.second->transform.scale, itemMenuItem.second->color, itemMenuItem.second->GetFontPath(), "x" + std::to_string(itemCount[itemMenuItem.second->GetText()]));
                itemCountMenuItem->Render(renderer, camera);
            }
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
    DrawBorders(renderer, camera);
}

void BattleMenu::OnCollision(std::shared_ptr<GameObject> collidedObj, glm::vec2 collisionNormal, float dt)
{
}

void BattleMenu::AddAttackMenuItem(std::string name, glm::vec3 position, glm::vec3 scale, glm::vec4 color, std::string fontPath, std::string text)
{
    std::shared_ptr<MenuItem> menuItem = std::make_shared<MenuItem>(name, position, scale, color, fontPath, text);
    attackMenuItems[text] = menuItem;
    menuItemsMap["AttackMenuItems"] = attackMenuItems;
}

void BattleMenu::AddMagicMenuItem(std::string name, glm::vec3 position, glm::vec3 scale, glm::vec4 color, std::string fontPath, std::string text)
{
    std::shared_ptr<MenuItem> menuItem = std::make_shared<MenuItem>(name, position, scale, color, fontPath, text);
    magicMenuItems[text] = menuItem;
    menuItemsMap["MagicMenuItems"] = attackMenuItems;
}

void BattleMenu::AddItemMenuItem(std::string name, glm::vec3 position, glm::vec3 scale, glm::vec4 color, std::string fontPath, std::string text)
{
    std::shared_ptr<MenuItem> menuItem = std::make_shared<MenuItem>(name, position, scale, color, fontPath, text);
    itemMenuItems[text] = menuItem;
    menuItemsMap["ItemsMenuItems"] = attackMenuItems;
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

void BattleMenu::UpdatePlayerMP(int mp)
{
    menuItems["menuItemPlayerMP"]->ChangeText("MP: " + std::to_string(mp));
}

void BattleMenu::SetItemCount(std::string item, int count)
{
    itemCount[item] = count;
}
