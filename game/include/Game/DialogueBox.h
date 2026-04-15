#pragma once

#include <Engine/Scene/GameObject.h>
#include <Game/MenuItem.h>


class Menu;
class DialogueTree;

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
    void SetMenuActive(bool active);
    void SetCurrentText(std::shared_ptr<MenuItem> currentText){this->currentText = currentText;}
    
    int GetIndex(){return index;}
    void SetIndex(int index){this->index = index;}

    std::vector<std::string> GetDialogue(){return dialogue;}

    void SetTextPosition(glm::vec3 position);
    void SetBordersPosition(glm::vec3 position);

    void AddSelectMenuItem(std::string name, glm::vec3 position, glm::vec3 scale, glm::vec4 color, std::string fontPath, std::string text);
    void AddSelectMenu(glm::vec3 position, glm::vec3 scale, glm::vec4 color, std::string texturePath, std::string name);
    std::shared_ptr<Menu> GetSelectMenu(){return this->selectMenu;}

    void SetDialogueTree(std::vector<std::unordered_map<std::string, std::vector<std::string>>> dialogueTree);
    void ResetDialogue(int indexOfD, std::string dialogueOption);
    
    std::string DialogueResponse(std::string response);
    void ClearDialogueResponses(){this->dialogueResponses.clear();}

private:
    bool inUse = false;
    int index = 0;
    int indexRendered = 0;
    std::vector<std::string> dialogue;
    std::vector<glm::vec3> dialogueScale;
    std::shared_ptr<MenuItem> currentText;
    std::vector<std::shared_ptr<GameObject>> borders;

    std::shared_ptr<Menu> selectMenu;
    bool selectMenuActive = false;
    std::unordered_map<std::string, std::string> dialogueResponses;

    std::vector<std::unordered_map<std::string, std::vector<std::string>>> dialogueTree;

    void CreateBorders();
};