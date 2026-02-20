#include <Game/DialogueBox.h>
#include <Game/MenuItem.h>
#include <Game/Obstacle.h>
#include <Engine/Scene/LetterText.h>

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
        currentText->ChangeText(dialogue[0]);
        for(int i = 0; i < dialogue.size(); i++)
        {
            currentText->ChangeText(dialogue[i]);
            std::string dialogueStr = "";
            std::string dialogueLongStr = "";
            std::string dialogueWord = "";
            bool tooLong = false;
            for(int j = 0; j < currentText->GetLetters().size(); j++)
            {

                if(!tooLong && currentText->GetLetters()[j]->GetPosition().x + currentText->GetLetters()[j]->transform.scale.x > transform.position.x + (transform.scale.x / 2))
                {
                    tooLong = true;
                }

                if(currentText->GetLetters()[j]->GetText() != " ")
                {
                    dialogueWord += dialogue[i][j];
                }
                else
                {
                    if(!tooLong)
                    {
                        dialogueStr += dialogueWord;
                    }
                    else
                    {
                        dialogueLongStr += dialogueWord;
                    }
                    dialogueWord = " ";
                }
                if(j == currentText->GetLetters().size() - 1 && tooLong)
                {
                    dialogueLongStr += dialogueWord;
                    dialogueLongStr.erase(0, 1);
                }
            }
            if(dialogueLongStr.length() != 0)
            {
                dialogue[i] = dialogueStr;
                if(i + 1 < dialogue.size())
                    dialogue.insert(dialogue.begin() + (i + 1), dialogueLongStr);
                else
                    dialogue.push_back(dialogueLongStr);
            }
        }
        currentText->ChangeText(dialogue[0]);
        this->dialogue = dialogue;
    }
}

void DialogueBox::SetTextPosition(glm::vec3 position)
{
    position.x -= transform.scale.x / 2;
    position.x += currentText->GetFontSize();
    currentText->SetPosition(position);
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

void DialogueBox::SetBordersPosition(glm::vec3 position)
{
    borders.clear();
    CreateBorders();
}
