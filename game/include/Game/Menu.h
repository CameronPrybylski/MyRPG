#pragma once

#include <Engine/Scene/GameObject.h>
#include <map>

class MenuItem;

class Menu : public GameObject {

public:

    Menu(glm::vec3 position, glm::vec3 scale, glm::vec4 color, std::string texturePath, std::string name);
    ~Menu();

    void OnEvent(const Input& input) override;
    void Update(const Input& input, float dt) override;
    void Render(Renderer& renderer, const Camera& camera) override;
    void OnCollision(std::shared_ptr<GameObject> collidedObj, glm::vec2 collisionNormal, float dt) override;

    virtual void AddMenuItem(std::string name, glm::vec3 position, glm::vec3 scale, glm::vec4 color, std::string fontPath, std::string text);
    virtual void AddCursor(std::string name, glm::vec3 position, glm::vec3 scale, glm::vec4 color, std::string texturePath);

    virtual void SetCursorMinHeight(float height)
    {
        cursorMinHeight = height;
    }
    virtual void SetCursorMaxHeight(float height)
    {
        cursorMaxHeight = height;
    }

    float GetCursorMaxHeight()
    {
        return cursorMaxHeight;
    }
    float GetCursorMinHeight()
    {
        return cursorMinHeight;
    }

    bool GetActive(){return active;}
    void SetActive(bool active){this->active = active;}

    std::shared_ptr<GameObject> GetCursor(){return cursor;}


    std::shared_ptr<MenuItem> GetMenuItem(std::string name){return menuItems[name];}

    virtual void SetMenuItemsSize();

    virtual void UpdateMenuItems(std::map<std::string, int> menuItemsMap);

    virtual void DrawBorders(Renderer &renderer, const Camera &camera);


protected:

    std::unordered_map<std::string, std::shared_ptr<MenuItem>> menuItems;
    std::shared_ptr<GameObject> cursor;
    float cursorMinHeight;
    float cursorMaxHeight;
    std::string menuName;
    std::vector<std::shared_ptr<GameObject>> borders;
    bool active = true;

    void CreateBorders();

    
};