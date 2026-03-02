#pragma once

class Scene;
class Menu;
class PlayerMenu;

class MenuScene : public Scene {
public:
    MenuScene(float screenWidth, float screenHeight, std::string filepath, std::string battleFilePath, std::string root);
    ~MenuScene();

    void Init() override;

    void OnEvent(const Input& input) override;
    void OnUpdate(const Input& input, PhysicsSystem& physics, float dt) override;

    void LoadMenuScene();

    void PlayerInfo();
    void SetPlayerInfo();
    void SetPlayerMenu();

    void UseItem();

private:
    std::string nextScene;
    std::string filepath;
    std::string battleFilePath;
    std::shared_ptr<PlayerMenu> menu;
    std::shared_ptr<Menu> background;
    std::shared_ptr<Menu> activeMenu;
    std::map<std::string, int> playerInfoMap;
    std::unordered_map<std::string, int> items;
    
    int maxHP;
    int xpNeeded;

    std::string root;
};