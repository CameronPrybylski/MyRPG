#pragma once

class Scene;
class Menu;

class SaveScene : public Scene {

public:
    SaveScene(float screenWidth, float screenHeight, std::string filepath, std::string saveFilePath);
    ~SaveScene();

    void Init() override;

    void OnEvent(const Input& input) override;
    void OnUpdate(const Input& input, PhysicsSystem& physics, float dt) override;

private:
    std::string saveFilePath;
};