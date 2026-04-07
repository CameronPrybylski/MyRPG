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
                    if(menuName == "ItemsMenuItems")
                    {
                        ResetCursorMinMax(itemsMenuItems);
                    }
                    else if(menuName == "EquipmentMenuItems")
                    {
                        ResetCursorMinMax(equipmentMenuItems);
                    }
                    break;
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
                        ResetCursorMinMax(menuItems);
                    }
                    break;
                }
            }
        }
        else if(menuName == "EquipmentMenuItems")
        {
            for(auto equipmentMenuItem : equipmentMenuItems)
            {
                if(cursor->transform.position.y == equipmentMenuItem.second->transform.position.y)
                {
                    if(equipmentMenuItem.second->GetText() != "Back")
                    {
                        playerMove = "ChangeWeapon" + equipmentMenuItem.second->GetText();
                    }
                    menuName = "MenuItems";
                    ResetCursorMinMax(menuItems);
                    break;
                }
            }
        }
        else
        {
            menuName = "MenuItems";
            ResetCursorMinMax(menuItems);
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
            if(itemsMenuItem.first.find("Back") == std::string::npos && descriptions.empty())
            {
                std::unique_ptr<MenuItem> itemCountMenuItem = std::make_unique<MenuItem>("ItemCount", itemsMenuItem.second->GetEndPosition() + glm::vec3(25.0f, 0.0f, 0.0f), itemsMenuItem.second->transform.scale, itemsMenuItem.second->color, itemsMenuItem.second->GetFontPath(), "x" + std::to_string(items[itemsMenuItem.second->GetText()]));
                itemCountMenuItem->Render(renderer, camera);
            }
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

void PlayerMenu::AddEquipmentMenuItem(std::string name, glm::vec3 position, glm::vec3 scale, glm::vec4 color, std::string fontPath, std::string text)
{
    std::shared_ptr<MenuItem> menuItem = std::make_shared<MenuItem>(name, position, scale, color, fontPath, text);
    equipmentMenuItems[text] = menuItem;
}

void PlayerMenu::SetItemsMenuItemsSize()
{
    
}

std::string PlayerMenu::CurrentItem()
{
    std::unordered_map<std::string, std::shared_ptr<MenuItem>> currentMenuItems;
    if(menuName == "MenuItems")
    {
        return "";
    }
    else if(menuName == "ItemsMenuItems")
    {
        currentMenuItems = itemsMenuItems;
    }
    else if(menuName == "EquipmentMenuItems")
    {
        currentMenuItems = equipmentMenuItems;
    }
    for(auto currentMenuItem : currentMenuItems)
    {
        if(cursor->transform.position.y == currentMenuItem.second->transform.position.y)
        {
            return currentMenuItem.second->GetText();
        }
    }
    return "";
}

void PlayerMenu::ChangeDescription()
{
    if(!descriptions.empty() && menuName != "MenuItems")
    {
        std::unordered_map<std::string, std::shared_ptr<MenuItem>> currentMenuItems;
        if(menuName == "ItemsMenuItems")
        {
            currentMenuItems = itemsMenuItems;
        }
        else if(menuName == "EquipmentMenuItems")
        {
            currentMenuItems = equipmentMenuItems;
        }
        if(description.length())
            currentMenuItems.begin()->second->ChangeText(description);
    }
}