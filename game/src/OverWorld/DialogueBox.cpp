#include <Game/DialogueBox.h>
#include <Game/MenuItem.h>
#include <Game/Menu.h>
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
        if(index == dialogue.size() - 1)
        {
            selectMenuActive = true;
            selectMenu->SetActive(true);
        }
        else
        {
            selectMenuActive = false;
            selectMenu->SetActive(false);
        }
    }
    if(selectMenuActive && selectMenu != nullptr)
    {
        selectMenu->OnEvent(input);
        if(input.IsKeyDown("RETURN") )
        {
            for(int i = 0; i < dialogueTree.size(); i++)
            {
                std::string selectMenuName = selectMenu->GetMenuName();
                std::string strMenuName = "MenuItems";
                size_t end = selectMenuName.find(strMenuName);
                std::string dialogueOption = selectMenuName.substr(0, end);
                if(dialogueTree[i].count(dialogueOption) && dialogueOption != "")
                {
                    ResetDialogue(i, dialogueOption);
                    std::string dialogueResponseKey = "";
                    std::string dialogueResponseValue = "";
                    size_t space = dialogueOption.find(" ");
                    if(space != std::string::npos)
                    {
                        dialogueResponseKey = dialogueOption.substr(0, space);
                        dialogueResponseValue = dialogueOption.substr(space + 1, dialogueOption.length());
                        this->dialogueResponses[dialogueResponseKey] = dialogueResponseValue;
                    }
                    break;
                }
            }
        }
    }
}

void DialogueBox::ResetDialogue(int indexOfD, std::string dialogueOption)
{
    dialogue = dialogueTree[indexOfD][dialogueOption];
    this->index = 0;
    currentText->ChangeText(dialogue[this->index]);
    selectMenuActive = false;
    selectMenu->SetActive(false);

    std::vector<std::string> newMenuItemsText;
    
    if(2*indexOfD + 1 < dialogueTree.size())
    {
        newMenuItemsText.push_back(dialogueTree[2*indexOfD + 1].begin()->first);
        newMenuItemsText.push_back(dialogueTree[2*indexOfD + 2].begin()->first);
    }
    else
    {
        selectMenu->SetDefaultMenuName();
        newMenuItemsText.push_back(dialogueTree[1].begin()->first);
        newMenuItemsText.push_back(dialogueTree[2].begin()->first);
    }

    selectMenu->SetMenuItemsText(newMenuItemsText);

}

std::string DialogueBox::DialogueResponse(std::string response)
{
    if(this->dialogueResponses.count(response))
    {
        return dialogueResponses.at(response);
    }
    return "";
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
        if(selectMenuActive && selectMenu != nullptr)
            selectMenu->Render(renderer, camera);
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

void DialogueBox::AddSelectMenuItem(std::string name, glm::vec3 position, glm::vec3 scale, glm::vec4 color, std::string fontPath, std::string text)
{
    selectMenu->AddMenuItem(name, position, scale, color, fontPath, text);
}

void DialogueBox::AddSelectMenu(glm::vec3 position, glm::vec3 scale, glm::vec4 color, std::string texturePath, std::string name)
{
    selectMenu = std::make_shared<Menu>(position, scale, color, "", name);
}

void DialogueBox::SetMenuActive(bool active)
{
    this->selectMenu->SetActive(active);
}

void DialogueBox::SetDialogueTree(std::vector<std::unordered_map<std::string, std::vector<std::string>>> dialogueTree)
{
    this->dialogueTree = dialogueTree;
}
