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
    }

}

void Menu::Update(const Input &input, float dt)
{
    //std::cout << cursor->transform.position.y << std::endl;
}

void Menu::Render(Renderer &renderer, const Camera &camera)
{
    /*
    renderer.DrawQuad(*mesh, transform, camera, AssetManager::GetShader(shaderName), color);
    if(menuName == "MenuItems")
    {
        for(auto menuItem : menuItems)
        {
            menuItem.second->Render(renderer, camera);
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
    */
    for(auto border : borders)
    {
        border->Render(renderer, camera);
    }
}

void Menu::OnCollision(std::shared_ptr<GameObject> collidedObj, glm::vec2 collisionNormal, float dt)
{
}

void Menu::AddMenuItem(std::string name, glm::vec3 position, glm::vec3 scale, glm::vec4 color, std::string fontPath, std::string text)
{
    std::shared_ptr<MenuItem> menuItem = std::make_shared<MenuItem>(name, position, scale, color, fontPath, text);
    menuItems[name] = menuItem;
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
    std::shared_ptr<MenuItem> currentText;
    float maxX = 0.0f;
    std::string maxXString = "";
    for(auto menuItem : menuItems)
    {
        currentText = menuItem.second;
        if(maxX <= currentText->GetText().length())
        {
            maxX = currentText->GetText().length();
            maxXString = menuItem.first;
        }
    }
    currentText = menuItems[maxXString];
    currentText->transform.scale.x = currentText->GetText().length() * 20;
    if(currentText->transform.scale.x <= (0.5) * transform.scale.x || currentText->transform.scale.x >= (0.75) * transform.scale.x)
    {
        currentText->transform.scale.x = (0.625) * transform.scale.x;
    }
    currentText->transform.scale.y = (currentText->transform.scale.x / 4);
    float yScale = currentText->transform.scale.y;
    float xScale = currentText->transform.scale.x;
    float widthToTextLength = currentText->GetText().length() / xScale;
    for(auto menuItem : menuItems)
    {
        currentText = menuItem.second;
        if(menuItem.first != maxXString)
        {
            currentText->transform.scale.y = yScale;
            currentText->transform.scale.x = currentText->GetText().length() / widthToTextLength;
        }
    }
}