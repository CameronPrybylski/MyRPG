#pragma once

#include <Engine/Scene/StringText.h>

class FinalFantasyText : public StringText 
{
public:
    FinalFantasyText(std::string letters, glm::vec3 position, glm::vec4 color, std::string fontPath, int fontSize);
    
    void SetTextSizeAndPos() override;

    void ChangeText(std::string newLetters) override;

};