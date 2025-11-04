#pragma once

#include <Engine/Scene/Scene.h>
#include <Game/Player.h>
#include <Game/Enemy.h>
#include <set>

class SaveSpot;

class Level : public Scene {

public:
    Level(float screenWidth, float screenHeight, std::string filepath, std::string saveFilePath, std::string saveBattleFilePath, std::string saveGameFilePath);
    //Level(std::unordered_map<std::string, std::shared_ptr<GameObject>> objects);
    ~Level();

    void Init() override;

    void LoadLevel(std::string filepath);
    void LoadPhysics(PhysicsSystem& physics) override;
    void OnEvent(const Input& input) override;
    void OnUpdate(const Input& input, PhysicsSystem& physics, float dt) override;
    void OnCollision(std::vector<CollisionEvent> collisions, float dt);

    void SaveState();
    void LoadState();

    void SaveGame(std::string saveSpot);
    void LoadGame();

    void RemoveEnemy(std::string enemyName);

    //void UpdatePhysics(PhysicsSystem& physics, float dt);

    void UpdateCamera();

    void SetSaveSlot(std::string saveSlot){this->saveSlot = saveSlot;}
    void SetLoadGame(bool loadGame){this->loadGame = loadGame;}

private:
    std::shared_ptr<Player> player;
    std::vector<std::shared_ptr<Enemy>> enemies; 
    std::unordered_map<std::string, std::shared_ptr<SaveSpot>> saveSpots;
    bool gameOver = false;
    std::string filepath;
    float completionDist;
    std::string nextLevel;
    std::string saveFilePath;
    std::string saveBattleFilePath;
    std::string saveGameFilePath;
    bool initialStart;
    std::set<std::string> deadEnemies;
    std::string savescene;
    bool loadGame = false;
    std::string saveSlot;

};