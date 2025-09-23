#pragma once

#include <Engine/Scene/Scene.h>

class GameOver : public Scene {
public:
    GameOver(float screenWidth, float screenHeight);
    ~GameOver(){}
    void OnEvent(const Input& input) override;
    void OnUpdate(const Input& input, PhysicsSystem& physics, float dt) override;
    void LoadPhysics(PhysicsSystem& physics) override;
    void Init() override;
};