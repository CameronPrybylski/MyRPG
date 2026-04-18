#include <Game/Menu.h>
#include <Game/MenuItem.h>
#include <Game/Obstacle.h>

Menu::Menu(glm::vec3 position, glm::vec3 scale, glm::vec4 color, std::string texturePath, std::string name)
{
    transform.position = position;
    transform.scale = scale;
    this->color = color;
    this->name = name;

    menuName = "MenuItems";
    menuItemsMap[menuName] = menuItems;

    mesh = AssetManager::GetMesh("quadMesh");

    if(texturePath != ""){
        shaderName = "textureShader";
        texture.Create(texturePath);
    }else{
        shaderName = "objectShader";
    }
    CreateBorders();

}

Menu::~Menu()
{
}

void Menu::OnEvent(const Input &input)
{
    if(input.IsKeyDown("W") || input.IsKeyDown("S"))
    {
        MoveCursor(input);
    }
    if(input.IsKeyDown("RETURN") && active)
    {
        if(menuName == "MenuItems")
        {
            for(auto menuItem : menuItems)
            {
                if(cursor->transform.position.y == menuItem.second->transform.position.y && menuItem.second->GetSelectable())
                {
                    menuName = menuItem.second->GetText() + "MenuItems";
                }
            }
        }
        else
        {
            menuName = "MenuItems";
        }
    }
    if(!active)
    {
        menuName = "MenuItems";
    }

}

void Menu::MoveCursor(const Input& input)
{
    if(!active)
    {
        return;
    }
    std::unordered_map<std::string, std::shared_ptr<MenuItem>> genericMenuItems;
    bool down = false;
    bool upOrDown = false;
    float minMax;
    if(menuItemsMap.count(menuName))
        genericMenuItems = menuItemsMap.at(menuName);
    else
        return;

    if(input.IsKeyDown("W") && cursor->transform.position.y < cursorMaxHeight && active)
    {
        upOrDown = true;
        minMax = cursorMaxHeight;
    }
    if(input.IsKeyDown("S") && cursor->transform.position.y > cursorMinHeight && active)
    {
        upOrDown = true;
        down = true;
        minMax = cursorMinHeight;
    }
    if(upOrDown)
    {
        for(auto item : genericMenuItems)
        {
            //Cursor move down aka 'S'
            if(down && item.second->GetSelectable() && cursor->transform.position.y > item.second->transform.position.y && item.second->transform.position.y > minMax)
            {
                minMax = item.second->transform.position.y;
            }
            //Cursor move up aka 'W'
            else if(item.second->GetSelectable() && cursor->transform.position.y < item.second->transform.position.y && item.second->transform.position.y < minMax)
            {
                minMax = item.second->transform.position.y;
            }
        }
        cursor->transform.position.y = minMax;
    }
}

void Menu::ResetCursorMinMax(std::unordered_map<std::string, std::shared_ptr<MenuItem>> menuItems)
{
    float minCursor = cursorMaxHeight;
    float maxCursor = cursorMinHeight;
    for(auto menuItem : menuItems)
    {
        if(menuItem.second->transform.position.y < minCursor)
        {
            minCursor = menuItem.second->transform.position.y;
        }
        if(menuItem.second->transform.position.y > maxCursor)
        {
            maxCursor = menuItem.second->transform.position.y;
        }
    }
    cursorMaxHeight = maxCursor;
    cursorMinHeight = minCursor;
    cursor->transform.position.y = cursorMaxHeight;
}

void Menu::Update(const Input &input, float dt)
{
    //std::cout << cursor->transform.position.y << std::endl;
}

void Menu::Render(Renderer &renderer, const Camera &camera)
{
    renderer.DrawQuad(*mesh, transform, camera, AssetManager::GetShader(shaderName), color);
    if(menuName == "MenuItems")
    {
        for(auto menuItem : menuItems)
        {
            menuItem.second->Render(renderer, camera);
        }
    }

    if(active)
        renderer.DrawQuad(*cursor->mesh, cursor->transform, camera, AssetManager::GetShader(cursor->shaderName), cursor->color);

    DrawBorders(renderer, camera);

}

void Menu::OnCollision(std::shared_ptr<GameObject> collidedObj, glm::vec2 collisionNormal, float dt)
{
}

void Menu::AddMenuItem(std::string name, glm::vec3 position, glm::vec3 scale, glm::vec4 color, std::string fontPath, std::string text)
{
    std::shared_ptr<MenuItem> menuItem = std::make_shared<MenuItem>(name, position, scale, color, fontPath, text);
    menuItems[name] = menuItem;
    menuItemsMap["MenuItems"] = menuItems;
    if(position.y < cursorMinHeight)
    {
        cursorMinHeight = position.y;
    }
    if(position.y > cursorMaxHeight)
    {
        cursorMaxHeight = position.y;
    }
}

void Menu::AddCursor(std::string name, glm::vec3 position, glm::vec3 scale, glm::vec4 color, std::string texturePath)
{
    cursor = std::make_shared<GameObject>();
    cursor->name = name;
    cursor->transform.position = position;
    cursor->transform.scale = scale;
    cursor->color = color;
    
    cursor->mesh = AssetManager::GetMesh("quadMesh");

    if(texturePath != ""){
        cursor->shaderName = "textureShader";
        cursor->texture.Create(texturePath);
    }else{
        cursor->shaderName = "objectShader";
    }

}

void Menu::DrawBorders(Renderer &renderer, const Camera &camera)
{
    for(auto border : borders)
    {
        border->Render(renderer, camera);
    }
}

void Menu::CreateBorders()
{
    bool secondHalf = false;
    glm::vec3 leftRightPos{(transform.scale.x / 2) - 2.5f, 0.0f, 0.0f};
    glm::vec3 upDownPos{0.0f, transform.scale.y / 2, 0.0f};

    glm::vec3 borderPos;
    glm::vec3 borderScale;
    glm::vec4 borderColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    for(int i = 0; i < 4; i++)
    {
        std::string borderName = "border" + std::to_string(i);
        if(i > 1 && !secondHalf)
        {
            leftRightPos *= -1;
            upDownPos    *= -1;
            secondHalf = true;
        }
        if(i % 2 == 0)
        {
            borderPos = transform.position + leftRightPos;
            borderScale = glm::vec3(5.0f, transform.scale.y, 0.0f);
        }
        else
        {
            borderPos = transform.position + upDownPos;
            borderScale = glm::vec3(transform.scale.x, 5.0f, 0.0f);
        }
        std::shared_ptr<GameObject> border = std::make_shared<Obstacle>(borderPos, borderScale, glm::vec3(0.0f), glm::vec3(0.0f), borderColor, "", borderName, true);
        borders.push_back(border);
    }
}

void Menu::SetMenuItemsSize()
{
}

void Menu::UpdateMenuItems(std::map<std::string, int> menuItemsMap)
{
    for(auto item : menuItemsMap)
    {
        if(menuItems.count(item.first))
        {
            menuItems[item.first]->ChangeText(item.first + ": " + std::to_string(item.second));
        }
    }

    SetMenuItemsSize();
}

void Menu::SetMenuItemsText(std::vector<std::string> newMenuItemsText)
{
    if(menuItems.size() == newMenuItemsText.size())
    {
        int i = 0;
        for(auto menuItem : menuItems)
        {
            if(newMenuItemsText[i] != "")
                menuItem.second->ChangeText(newMenuItemsText[i]);
            i++;
        }
        menuName = "MenuItems";
    }
}

void Menu::ChangePosition(glm::vec3 newPosition, float xDiff, float yDiff)
{
    this->transform.position = newPosition;
    borders.clear();
    CreateBorders();
    for(auto menuItem : menuItems)
    {
        menuItem.second->SetPosition(newPosition);
        if(newPosition.y > cursorMaxHeight)
        {
            cursorMaxHeight = newPosition.y;
        }
        if(newPosition.y < cursorMinHeight)
        {
            cursorMinHeight = newPosition.y;
        }
        newPosition = newPosition + glm::vec3(0.0f, yDiff, 0.0f);
    }
    cursor->transform.position = newPosition + glm::vec3(xDiff, -yDiff, 0.0f);
    ResetCursorMinMax(menuItems);
}