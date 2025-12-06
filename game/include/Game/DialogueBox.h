#pragma once

#include <Engine/Scene/GameObject.h>

class MenuItem;

class DialogueBox : public GameObject{

public:
    DialogueBox(glm::vec3 position, glm::vec3 scale, glm::vec4 color, std::string texturePath, std::string name);
    ~DialogueBox();

    void OnEvent(const Input& input) override;
    void Update(const Input& input, float dt) override;
    void Render(Renderer& renderer, const Camera& camera) override;

    void SetDialogue(std::vector<std::string> dialogue);
    void SetInUse(bool inUse){this->inUse = inUse;}
    bool InUse(){return inUse;}
    void SetCurrentText(std::shared_ptr<MenuItem> currentText){this->currentText = currentText;}
    
    int GetIndex(){return index;}
    void SetIndex(int index){this->index = index;}

    std::vector<std::string> GetDialogue(){return dialogue;}


private:
    bool inUse = false;
    int index = 0;
    int indexRendered = 0;
    std::vector<std::string> dialogue;
    std::vector<glm::vec3> dialogueScale;
    std::shared_ptr<MenuItem> currentText;
    std::vector<std::shared_ptr<GameObject>> borders;

    void CreateBorders();
    void ResizeDialogue();
};