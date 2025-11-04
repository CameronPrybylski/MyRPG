#pragma once 

#include <Game/Menu.h>

class MenuItem;

class SaveMenu : public Menu {

public:
    SaveMenu(glm::vec3 position, glm::vec3 scale, glm::vec4 color, std::string texturePath, std::string name);
    ~SaveMenu();

    void OnEvent(const Input& input) override;
    void Update(const Input& input, float dt) override;
    void Render(Renderer& renderer, const Camera& camera) override;
    void OnCollision(std::shared_ptr<GameObject> collidedObj, glm::vec2 collisionNormal, float dt) override;


    void AddSaveMenuItem(std::string name, glm::vec3 position, glm::vec3 scale, glm::vec4 color, std::string fontPath, std::string text);

    bool SaveGame(){return saveGame;}
    std::string SaveSlot(){return saveSlot;}
    void SetSaveGame(bool saveGame){this->saveGame = saveGame;}

private:
    std::unordered_map<std::string, std::shared_ptr<MenuItem>> saveMenuItems;
    bool saveGame = false;
    std::string saveSlot = "";
};