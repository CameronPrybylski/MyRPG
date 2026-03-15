#pragma once
#include <Game/Menu.h>


class MenuItem;

class BattleMenu : public Menu {

public:

    BattleMenu(glm::vec3 position, glm::vec3 scale, glm::vec4 color, std::string texturePath, std::string name);
    ~BattleMenu();

    void OnEvent(const Input& input) override;
    void Update(const Input& input, float dt) override;
    void Render(Renderer& renderer, const Camera& camera) override;
    void OnCollision(std::shared_ptr<GameObject> collidedObj, glm::vec2 collisionNormal, float dt) override;

    void AddAttackMenuItem(std::string name, glm::vec3 position, glm::vec3 scale, glm::vec4 color, std::string fontPath, std::string text);
    void AddMagicMenuItem(std::string name, glm::vec3 position, glm::vec3 scale, glm::vec4 color, std::string fontPath, std::string text);
    void AddItemMenuItem(std::string name, glm::vec3 position, glm::vec3 scale, glm::vec4 color, std::string fontPath, std::string text);

    std::string GetPlayerMove() const
    {
        return playerMove;
    }

    void SetPlayerMove(std::string move)
    {
        playerMove = move;
    }

    void SetDeadEnemies(std::vector<std::string> deadEnemies);

    void UpdatePlayerHP(int hp);

    void RemoveItemMenuItem(std::string menuItem){itemMenuItems.erase(menuItem);}

    void SetItemCount(std::string item, int count);


private:

    std::unordered_map<std::string, std::shared_ptr<MenuItem>> attackMenuItems;
    std::unordered_map<std::string, std::shared_ptr<MenuItem>> magicMenuItems;
    std::unordered_map<std::string, std::shared_ptr<MenuItem>> itemMenuItems;
    std::unordered_map<std::string, int> itemCount;
    std::string playerMove = "";

};