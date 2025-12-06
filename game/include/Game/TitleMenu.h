#pragma once 

#include <Game/Menu.h>

class MenuItem;

class TitleMenu : public Menu {

public:
    TitleMenu(glm::vec3 position, glm::vec3 scale, glm::vec4 color, std::string texturePath, std::string name);
    ~TitleMenu();

    void OnEvent(const Input& input) override;
    void Update(const Input& input, float dt) override;
    void Render(Renderer& renderer, const Camera& camera) override;
    void OnCollision(std::shared_ptr<GameObject> collidedObj, glm::vec2 collisionNormal, float dt) override;


    void AddLoadMenuItem(std::string name, glm::vec3 position, glm::vec3 scale, glm::vec4 color, std::string fontPath, std::string text);

    bool IsNewGame(){return newGame;}
    bool IsLoadGame(){return loadGame;}

    void SetMenuItemsSize() override;
private:
    std::unordered_map<std::string, std::shared_ptr<MenuItem>> loadMenuItems;
    bool newGame = false;
    bool loadGame = false;
};