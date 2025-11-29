#include <Game/DialogueBox.h>
#include <Game/MenuItem.h>

DialogueBox::DialogueBox(glm::vec3 position, glm::vec3 scale, glm::vec4 color, std::string texturePath, std::string name)
{
    this->transform.position = position;
    this->transform.scale = scale;
    this->name = name;
    this->color = color;
    this->mesh = AssetManager::GetMesh("quadMesh");
    this->dialogue = {};
    if(texturePath != ""){
        shaderName = "textureShader";
        texture.Create(texturePath);
    }else{
        shaderName = "objectShader";
    }
}

DialogueBox::~DialogueBox()
{
}

void DialogueBox::OnEvent(const Input &input)
{
    if(input.IsKeyDown("SPACE") && inUse && index == indexRendered)
    {
        index++;
        if(index < dialogue.size())
        {
            ResizeDialogue(index, dialogue);
        }
    }
}

void DialogueBox::Update(const Input &input, float dt)
{
}

void DialogueBox::Render(Renderer &renderer, const Camera &camera)
{
    if(inUse)
    {
        renderer.DrawQuad(*mesh, transform, camera, AssetManager::GetShader(shaderName), color);
        currentText->Render(renderer, camera);
        indexRendered = index;
    }
}

void DialogueBox::SetDialogue(std::vector<std::string> dialogue)
{
    this->dialogue = dialogue;
    if(dialogue.size() > 0)
    {
        ResizeDialogue(0, dialogue);
    }
}

void DialogueBox::ResizeDialogue(int index, std::vector<std::string> dialogue)
{
    currentText->ChangeText(dialogue[index]);
    if(currentText->transform.scale.x >= transform.scale.x || currentText->transform.scale.y >= transform.scale.y)
    {
        currentText->transform.scale.x = (0.75) * transform.scale.x;
        currentText->transform.scale.y = (0.50) * (transform.scale.y / 2);
    }
}
