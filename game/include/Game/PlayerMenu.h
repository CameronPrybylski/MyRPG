#pragma once

#include <Game/Menu.h>

class MenuItem;

class PlayerMenu : public Menu
{
public:
    PlayerMenu(glm::vec3 position, glm::vec3 scale, glm::vec4 color, std::string texturePath, std::string name);
    ~PlayerMenu();

    void OnEvent(const Input& input) override;
    void Update(const Input& input, float dt) override;
    void Render(Renderer& renderer, const Camera& camera) override;

    void AddItemsMenuItem(std::string name, glm::vec3 position, glm::vec3 scale, glm::vec4 color, std::string fontPath, std::string text);
    void SetItemsMenuItemsSize();

    std::string GetPlayerMove(){return playerMove;}
    void SetPlayerMove(std::string playerMove){this->playerMove = playerMove;}
    
private:
    std::unordered_map<std::string, std::shared_ptr<MenuItem>> itemsMenuItems;
    std::unordered_map<std::string, std::shared_ptr<MenuItem>> equipmentMenuItems;
    std::string playerMove;
};