#include <Game/PlayerMenu.h>
#include <Game/MenuItem.h>

PlayerMenu::PlayerMenu(glm::vec3 position, glm::vec3 scale, glm::vec4 color, std::string texturePath, std::string name) : Menu(position, scale, color, texturePath, name)
{
}

PlayerMenu::~PlayerMenu()
{
}
void PlayerMenu::OnEvent(const Input &input)
{
    if(input.IsKeyDown("W") && cursor->transform.position.y < cursorMaxHeight && active)
    {
        cursor->transform.position.y += 20.0f;
    }
    if(input.IsKeyDown("S") && cursor->transform.position.y > cursorMinHeight && active)
    {
        cursor->transform.position.y -= 20.0f;
    }
    if(input.IsKeyDown("RETURN") && active)
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
        else if(menuName == "ItemsMenuItems")
        {
            for(auto itemsMenuItem : itemsMenuItems)
            {
                if(cursor->transform.position.y == itemsMenuItem.second->transform.position.y)
                {
                    if(itemsMenuItem.second->GetText() != "Back")
                    {
                        playerMove = "UseItem" + itemsMenuItem.second->GetText();
                    }
                    else if(itemsMenuItem.second->GetText() == "Back")
                    {
                        menuName = "MenuItems";
                        playerMove = "";
                    }
                }
            }
        }
        /*
        else if(menuName == "EquipmentMenuItems")
        {
            for(auto itemMenuItem : itemMenuItems)
            {
                if(cursor->transform.position.y == itemMenuItem.second->transform.position.y)
                {
                    if(itemMenuItem.second->GetText() != "Back")
                    {
                        playerMove = "UseItem" + itemMenuItem.second->GetText();
                    }
                    menuName = "MenuItems";
                }
            }
        }
        */
       else
       {
            menuName = "MenuItems";
       }
    }
}

void PlayerMenu::Update(const Input &input, float dt)
{
}

void PlayerMenu::Render(Renderer &renderer, const Camera &camera)
{
    Menu::Render(renderer, camera);
    if(menuName == "ItemsMenuItems")
    {
        for(auto itemsMenuItem : itemsMenuItems)
        {
            itemsMenuItem.second->Render(renderer, camera);
        }
    }
    else if(menuName == "EquipmentMenuItems")
    {
        for(auto equipmentMenuItem : equipmentMenuItems)
        {
            equipmentMenuItem.second->Render(renderer, camera);
        }
    }
}

void PlayerMenu::AddItemsMenuItem(std::string name, glm::vec3 position, glm::vec3 scale, glm::vec4 color, std::string fontPath, std::string text)
{
    std::shared_ptr<MenuItem> menuItem = std::make_shared<MenuItem>(name, position, scale, color, fontPath, text);
    itemsMenuItems[text] = menuItem;
}

void PlayerMenu::SetItemsMenuItemsSize()
{
    
}