#pragma once 

#include <Engine/Scene/GameObject.h>
#include <Engine/Renderer/TextTexture.h>

class StringText;
class LetterText;


class MenuItem : public GameObject {

public:
    MenuItem(std::string name, glm::vec3 position, glm::vec3 scale, glm::vec4 color, std::string fontPath, std::string text);
    ~MenuItem();

    void Render(Renderer& renderer, const Camera& camera) override;

    void SetTexture(std::string fontPath, std::string text);
    
    std::string GetText() const 
    {
        return text;
    }

    void ChangeText(std::string newText);

    void SetPosition(glm::vec3 position);

    std::string GetFontPath(){return fontPath;}

    std::string GetText(){return text;}

    int GetFontSize();

    glm::vec3 GetEndPosition();

    std::vector<std::shared_ptr<LetterText>> GetLetters();

    void SetSelectable(bool selectable){this->selectable = selectable;}
    bool GetSelectable(){return this->selectable;}

private:
    TextTexture textTexture;
    std::string text;
    std::string fontPath;
    bool selectable = true;

    std::shared_ptr<StringText> stringText;

};