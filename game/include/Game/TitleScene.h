#pragma once

class Scene;
class TitleMenu;
class MenuItem;

class TitleScene : public Scene {

public:
    TitleScene(float screenWidth, float screenHeight, std::string filepath, std::string saveFilePath, std::string root);
    ~TitleScene();

    void Init() override;

    void LoadTitle();
    void LoadPhysics(PhysicsSystem& physics) override;
    void OnEvent(const Input& input) override;
    void OnUpdate(const Input& input, PhysicsSystem& physics, float dt) override;
    void OnCollision(std::vector<CollisionEvent> collisions, float dt);

    void UpdateCamera();

    void LoadGame();
    void NewGame();

private:
    std::shared_ptr<TitleMenu> menu;
    std::shared_ptr<MenuItem> title;
    std::string saveFilePath;
    std::string filepath;
    bool initialStart = true;

    std::string root;

};