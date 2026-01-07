#pragma once

class Scene;
class Menu;

class MenuScene : public Scene {
public:
    MenuScene(float screenWidth, float screenHeight, std::string filepath, std::string battleFilePath);
    ~MenuScene();

    void Init() override;

    void OnEvent(const Input& input) override;
    void OnUpdate(const Input& input, PhysicsSystem& physics, float dt) override;

    void LoadMenuScene();

    void PlayerInfo();
    void SetPlayerMenu();

private:
    std::string nextScene;
    std::string filepath;
    std::string battleFilePath;
    std::shared_ptr<Menu> menu;
    std::shared_ptr<Menu> background;
    std::shared_ptr<Menu> activeMenu;
    std::map<std::string, int> playerInfoMap;
};