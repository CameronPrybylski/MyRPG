#include <Game/DialogueBox.h>
#include <Game/MenuItem.h>
#include <Game/Obstacle.h>

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
    CreateBorders();
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
            currentText->ChangeText(dialogue[index]);
            currentText->transform.scale = dialogueScale[index];
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
        for(auto border : borders)
        {
            border->Render(renderer, camera);
        }
        indexRendered = index;
    }
}

void DialogueBox::SetDialogue(std::vector<std::string> dialogue)
{
    this->dialogue = dialogue;
    if(dialogue.size() > 0)
    {
        //ResizeDialogue(0, dialogue);
        ResizeDialogue();
        currentText->ChangeText(dialogue[0]);
        currentText->transform.scale = dialogueScale[0];
    }
}

void DialogueBox::CreateBorders()
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

void DialogueBox::ResizeDialogue()
{
    currentText->ChangeText(dialogue[0]);
    float maxX = 0.0f;
    int maxXIndex = 0;
    for(int i = 0; i < dialogue.size(); i++)
    {
        currentText->ChangeText(dialogue[i]);
        if(maxX <= currentText->transform.scale.x)
        {
            maxX = currentText->transform.scale.x;
            maxXIndex = i;
        }
        dialogueScale.push_back(glm::vec3(0.0f));
    }
    currentText->ChangeText(dialogue[maxXIndex]);
    currentText->transform.scale.x = (0.75) * transform.scale.x;
    currentText->transform.scale.x = currentText->GetText().length() * 20;
    if(currentText->transform.scale.x <= (0.5) * transform.scale.x || currentText->transform.scale.x >= (0.75) * transform.scale.x)
    {
        currentText->transform.scale.x = (0.75) * transform.scale.x;
    }
    currentText->transform.scale.y = (currentText->transform.scale.x / 15);
    dialogueScale[maxXIndex] = currentText->transform.scale;
    float yScale = currentText->transform.scale.y;
    float xScale = currentText->transform.scale.x;
    float widthToTextLength = currentText->GetText().length() / xScale;
    for(int i = 0; i < dialogue.size(); i++)
    {
        currentText->ChangeText(dialogue[i]);
        if(i != maxXIndex)
        {
            dialogueScale[i].y = yScale;
            dialogueScale[i].x = currentText->GetText().length() / widthToTextLength;
        }
    }
}
