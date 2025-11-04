#include <Game/SaveMenu.h>
#include <Game/MenuItem.h>

SaveMenu::SaveMenu(glm::vec3 position, glm::vec3 scale, glm::vec4 color, std::string texturePath, std::string name)
 : Menu(position, scale, color, texturePath, name )
{
}

SaveMenu::~SaveMenu()
{
}

void SaveMenu::OnEvent(const Input &input)
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
                if(cursor->transform.position.y == menuItem.second->transform.position.y && menuItem.first == "Back")
                {
                    saveSlot = menuItem.second->GetText();
                }
                else if(cursor->transform.position.y == menuItem.second->transform.position.y)
                {
                    //menuName = menuItem.second->GetText() + "MenuItems";
                    saveGame = true;
                    saveSlot = menuItem.second->GetText();
                }
            }
        }
    }

}

void SaveMenu::Update(const Input &input, float dt)
{

}

void SaveMenu::Render(Renderer &renderer, const Camera &camera)
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
    
    if(cursor->transform.position.y < minHeight || cursor->transform.position.y > maxHeight)
    {
        cursor->transform.position.y = maxHeight;
    }
    renderer.DrawQuad(*cursor->mesh, cursor->transform, camera, AssetManager::GetShader(cursor->shaderName), cursor->color);
}

void SaveMenu::OnCollision(std::shared_ptr<GameObject> collidedObj, glm::vec2 collisionNormal, float dt)
{
}

void SaveMenu::AddSaveMenuItem(std::string name, glm::vec3 position, glm::vec3 scale, glm::vec4 color, std::string fontPath, std::string text)
{
    std::shared_ptr<MenuItem> menuItem = std::make_shared<MenuItem>(name, position, scale, color, fontPath, text);
    saveMenuItems[text] = menuItem;
}

