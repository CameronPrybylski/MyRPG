#include <Game/TitleMenu.h>
#include <Game/MenuItem.h>

TitleMenu::TitleMenu(glm::vec3 position, glm::vec3 scale, glm::vec4 color, std::string texturePath, std::string name)
 : Menu(position, scale, color, texturePath, name )
{
}

TitleMenu::~TitleMenu()
{
}

void TitleMenu::OnEvent(const Input &input)
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
        else if(menuName == "Load GameMenuItems")
        {
            for(auto loadMenuItem : loadMenuItems)
            {
                if(cursor->transform.position.y == loadMenuItem.second->transform.position.y)
                {
                    if(loadMenuItem.second->GetText() != "Back")
                    {
                        //playerMove = "Attack" + attackMenuItem.second->GetText();
                    }
                    //menuName = "MenuItems";
                }
            }
            menuName = "MenuItems";
        }
        else if(menuName == "New GameMenuItems")
        {
            newGame = true;
        }
    }

}

void TitleMenu::Update(const Input &input, float dt)
{
    if(menuName == "New GameMenuItems")
    {
        newGame = true;
    }
    if(menuName == "Load GameMenuItems")
    {
        loadGame = true;
    }
}

void TitleMenu::Render(Renderer &renderer, const Camera &camera)
{
    renderer.DrawQuad(*mesh, transform, camera, AssetManager::GetShader(shaderName), color);
    float minHeight = cursorMaxHeight;
    float maxHeight = cursorMinHeight;
    if(menuName == "MenuItems")
    {
        for(auto menuItem : menuItems)
        {
            if(menuItem.second->transform.position.y < minHeight)
            {
                SetCursorMinHeight(menuItem.second->transform.position.y);
                minHeight = menuItem.second->transform.position.y;
            }
            if(menuItem.second->transform.position.y > maxHeight)
            {
                SetCursorMaxHeight(menuItem.second->transform.position.y);
                maxHeight = menuItem.second->transform.position.y;
            }
            menuItem.second->Render(renderer, camera);
        }
    }
    else if(menuName == "Load GameMenuItems")
    {
        for(auto loadMenuItem : loadMenuItems)
        {
            loadMenuItem.second->Render(renderer, camera);
            if(loadMenuItem.second->transform.position.y < minHeight)
            {
                SetCursorMinHeight(loadMenuItem.second->transform.position.y);
                minHeight = loadMenuItem.second->transform.position.y;
            }
            if(loadMenuItem.second->transform.position.y > maxHeight)
            {
                SetCursorMaxHeight(loadMenuItem.second->transform.position.y);
                maxHeight = loadMenuItem.second->transform.position.y;
            }
        }
    }
    else
    {
        //menuName = "MenuItems";
        for(auto menuItem : menuItems)
        {
            menuItem.second->Render(renderer, camera);
        }
    }
    if(cursor->transform.position.y < minHeight || cursor->transform.position.y > maxHeight)
    {
        cursor->transform.position.y = maxHeight;
    }
    renderer.DrawQuad(*cursor->mesh, cursor->transform, camera, AssetManager::GetShader(cursor->shaderName), cursor->color);
    Menu::Render(renderer, camera);
}

void TitleMenu::OnCollision(std::shared_ptr<GameObject> collidedObj, glm::vec2 collisionNormal, float dt)
{
}

void TitleMenu::AddLoadMenuItem(std::string name, glm::vec3 position, glm::vec3 scale, glm::vec4 color, std::string fontPath, std::string text)
{
    std::shared_ptr<MenuItem> menuItem = std::make_shared<MenuItem>(name, position, scale, color, fontPath, text);
    loadMenuItems[text] = menuItem;
}

void TitleMenu::SetMenuItemsSize()
{
    std::shared_ptr<MenuItem> currentText;
    float maxX = 0.0f;
    std::string maxXString = "";
    int i = 0;
    for(auto menuItem : loadMenuItems)
    {
        currentText = menuItem.second;
        if(maxX <= currentText->GetText().length())
        {
            maxX = currentText->GetText().length();
            maxXString = menuItem.first;
        }
    }
    currentText = loadMenuItems[maxXString];
    currentText->transform.scale.x = transform.scale.x / 2.5;
    currentText->transform.scale.y = transform.scale.x / 100.0;
    //dialogueScale[maxXIndex] = currentText->transform.scale;
    float yScale = currentText->transform.scale.y;
    float xScale = currentText->transform.scale.x;
    float widthToTextLength = currentText->GetText().length() / xScale;
    i = 0;
    for(auto menuItem : loadMenuItems)
    {
        currentText = menuItem.second;
        if(menuItem.first != maxXString)
        {
            currentText->transform.scale.y = yScale;
            currentText->transform.scale.x = currentText->GetText().length() / widthToTextLength;
        }
    }
}

