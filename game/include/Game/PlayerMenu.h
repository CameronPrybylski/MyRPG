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

    void AddEquipmentMenuItem(std::string name, glm::vec3 position, glm::vec3 scale, glm::vec4 color, std::string fontPath, std::string text);

    std::string GetPlayerMove(){return playerMove;}
    void SetPlayerMove(std::string playerMove){this->playerMove = playerMove;}

    void SetItemCount(std::string item, int count)
    {
        items[item] = count;
    }
    void AddItem(std::string item)
    {
        if(items[item] > 0)
        {
            items[item]++;
        }
        else
        {
            items[item] = 1;
        }
    }
    void RemoveItem(std::string item)
    {
        if(items[item] > 1)
        {
            items[item]--;
        }
        else
        {
            items[item] = 0;
            itemsMenuItems.erase(item);
        }
    }

    void RemoveAllItems(std::string item)
    {
        items[item] = 0;
        itemsMenuItems.erase(item);
    }
    
private:
    std::unordered_map<std::string, std::shared_ptr<MenuItem>> itemsMenuItems;
    std::unordered_map<std::string, std::shared_ptr<MenuItem>> equipmentMenuItems;
    std::unordered_map<std::string, int> items;
    std::string playerMove;
    void ResetCursorMinMax(std::unordered_map<std::string, std::shared_ptr<MenuItem>> menuItems);
};