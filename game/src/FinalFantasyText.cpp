#include <Game/FinalFantasyText.h>

FinalFantasyText::FinalFantasyText(std::string letters, glm::vec3 position, glm::vec4 color, std::string fontPath, int fontSize) : StringText(letters, position, color, fontPath, fontSize)
{
}

void FinalFantasyText::SetTextSizeAndPos()
{
    glm::vec3 ogPosition = this->transform.position;
    for(int i = 0; i < letters.size(); i++)
    {
        letters[i]->SetPosition(this->transform.position);
        if(upperLetters.find(letters[i]->GetText()) != std::string::npos)
        {
            letters[i]->SetScale(glm::vec3(float(fontSize), float(fontSize)*3, 0.0f));
        }
        else
        {
            letters[i]->SetScale(glm::vec3(float(fontSize), float(fontSize), 0.0f));
        }

        if(letters[i]->GetText() == "I")
        {
            letters[i]->transform.scale.x /= 2;
            letters[i]->SetPosition(this->transform.position - glm::vec3{letters[i]->transform.scale.x / 2, 0.0f, 0.0f});
        }
        else if(letters[i]->GetText() == "L")
        {
            letters[i]->transform.scale.x *= 0.75f;
            letters[i]->SetPosition(this->transform.position - glm::vec3{letters[i]->transform.scale.x * 0.2f, 0.0f, 0.0f});
        }
        this->transform.position.x += letters[i]->transform.scale.x;
    }
    this->endPosition = this->transform.position;
    this->transform.position = ogPosition;
}