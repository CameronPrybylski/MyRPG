#pragma once

class Scene;
class EnemyInBattle;
class PlayerInBattle;
class BattleMenu;

class Battle : public Scene {

public:
    Battle(float screenWidth, float screenHeight, std::string filepath);
    ~Battle();

    void Init() override;

    void LoadBattle();
    void LoadPhysics(PhysicsSystem& physics) override;
    void OnEvent(const Input& input) override;
    void OnUpdate(const Input& input, PhysicsSystem& physics, float dt) override;
    void OnCollision(std::vector<CollisionEvent> collisions, float dt);

    void UpdateCamera();

    void HandlePlayerAttack(std::shared_ptr<EnemyInBattle> attacked);

private:
    std::string filepath;
    std::shared_ptr<PlayerInBattle> player;
    std::shared_ptr<BattleMenu> menu;
    std::string nextArea;
    std::unordered_map<std::string, std::shared_ptr<EnemyInBattle>> enemies;
    bool playerMove = true;

};