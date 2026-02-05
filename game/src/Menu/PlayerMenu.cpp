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
    std::shared_ptr<MenuItem> currentText;
    float maxX = 0.0f;
    std::string maxXString = "";
    for(auto menuItem : itemsMenuItems)
    {
        currentText = menuItem.second;
        if(maxX <= currentText->GetText().length())
        {
            maxX = currentText->GetText().length();
            maxXString = menuItem.first;
        }
    }
    currentText = itemsMenuItems[maxXString];
    currentText->transform.scale.x = currentText->GetText().length() * 20;
    if(/*currentText->transform.scale.x <= (0.5) * transform.scale.x ||*/ currentText->transform.scale.x >= (0.75) * transform.scale.x)
    {
        currentText->transform.scale.x = (0.625) * transform.scale.x;
    }
    else if(currentText->transform.scale.x < (0.625) * transform.scale.x && currentText->transform.scale.x > (0.4) * transform.scale.x)
    {
        currentText->transform.scale.x = (0.6) * transform.scale.x;
    }
    auto it = menuItems.begin();
    currentText->transform.scale.y = it->second->transform.scale.y; //(currentText->transform.scale.x / 4);
    float yScale = currentText->transform.scale.y;
    float xScale = currentText->transform.scale.x;
    float widthToTextLength = currentText->GetText().length() / xScale;
    
    for(auto menuItem : itemsMenuItems)
    {
        currentText = menuItem.second;
        if(menuItem.first != maxXString)
        {
            currentText->transform.scale.y = yScale;
            currentText->transform.scale.x = currentText->GetText().length() / widthToTextLength;
        }
    }
}