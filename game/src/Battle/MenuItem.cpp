#include <Game/MenuItem.h>

MenuItem::MenuItem(std::string name, glm::vec3 position, glm::vec3 scale, glm::vec4 color, std::string fontPath, std::string text)
{
    transform.position = position;
    transform.scale = scale;
    this->color = color;
    this->name = name;
    this->text = text;
    this->fontPath = fontPath;

    mesh = AssetManager::GetMesh("quadMesh");

    if(fontPath != ""){
        shaderName = "textureShader";
        textTexture.Create(fontPath, text);
        texture = textTexture;
    }else{
        shaderName = "objectShader";
    }
}

MenuItem::~MenuItem()
{
}

void MenuItem::Render(Renderer &renderer, const Camera &camera)
{
    renderer.DrawTexturedQuad(*mesh, transform, camera, AssetManager::GetShader("textureShader"), texture, color);
}

void MenuItem::SetTexture(std::string fontPath, std::string text)
{

}

void MenuItem::ChangeText(std::string newText)
{
    if(fontPath != ""){
        textTexture.Delete();
        shaderName = "textureShader";
        textTexture.Create(fontPath, newText);
        texture = textTexture;
        text = newText;
        transform.scale.x = 20.0 * ((double)text.length());
        transform.scale.y = 5.0 * ((double)text.length());
    }
}