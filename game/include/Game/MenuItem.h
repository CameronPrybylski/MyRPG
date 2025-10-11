#pragma once 

#include <Engine/Scene/GameObject.h>
#include <Engine/Renderer/TextTexture.h>


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

private:
    TextTexture textTexture;
    std::string text;
    std::string fontPath;

};