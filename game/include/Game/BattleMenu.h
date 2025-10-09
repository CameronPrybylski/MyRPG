#pragma once
#include <Engine/Scene/GameObject.h>

class MenuItem;

class BattleMenu : public GameObject {

public:

    BattleMenu(glm::vec3 position, glm::vec3 scale, glm::vec4 color, std::string texturePath, std::string name);
    ~BattleMenu();

    void OnEvent(const Input& input) override;
    void Update(const Input& input, float dt) override;
    void Render(Renderer& renderer, const Camera& camera) override;
    void OnCollision(std::shared_ptr<GameObject> collidedObj, glm::vec2 collisionNormal, float dt) override;

    void AddMenuItem(std::string name, glm::vec3 position, glm::vec3 scale, glm::vec4 color, std::string fontPath, std::string text);
    void AddAttackMenuItem(std::string name, glm::vec3 position, glm::vec3 scale, glm::vec4 color, std::string fontPath, std::string text);
    void AddCursor(std::string name, glm::vec3 position, glm::vec3 scale, glm::vec4 color, std::string texturePath);

    void SetCursorMinHeight(float height)
    {
        cursorMinHeight = height;
    }
    void SetCursorMaxHeight(float height)
    {
        cursorMaxHeight = height;
    }

    std::string GetPlayerMove() const
    {
        return playerMove;
    }

    void SetPlayerMove(std::string move)
    {
        playerMove = move;
    }


private:

    std::unordered_map<std::string, std::shared_ptr<MenuItem>> menuItems;
    std::unordered_map<std::string, std::shared_ptr<MenuItem>> attackMenuItems;
    std::shared_ptr<GameObject> cursor;
    float cursorMinHeight;
    float cursorMaxHeight;
    std::string playerMove = "";
    std::string menuName;

};