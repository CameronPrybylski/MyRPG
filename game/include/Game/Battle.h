#pragma once

class Scene;
class EnemyInBattle;
class PlayerInBattle;
class BattleMenu;

class Battle : public Scene {

public:
    Battle(float screenWidth, float screenHeight, std::string filepath, std::string saveFilePath, std::string saveGameFilePath, std::string root);
    ~Battle();

    void Init() override;

    void LoadBattle();
    void LoadPhysics(PhysicsSystem& physics) override;
    void OnEvent(const Input& input) override;
    void OnUpdate(const Input& input, PhysicsSystem& physics, float dt) override;
    void OnCollision(std::vector<CollisionEvent> collisions, float dt);

    void UpdateCamera();

    void HandlePlayerMove();
    void HandleEnemyMove(std::shared_ptr<EnemyInBattle> enemy);
    virtual std::shared_ptr<EnemyInBattle> CreateEnemy(glm::vec3 position, glm::vec3 scale, glm::vec4 color, std::string texturePath, std::string name, int attackDamage, int xp);
    void AddEnemy(std::shared_ptr<EnemyInBattle> enemy);

    void SavePlayerInfo();
    void LoadPlayerInfo();
    void LoadGame();

    void SetSaveSlot(std::string saveSlot){this->saveSlot = saveSlot;}
    void SetLoadGame(bool loadBattle){this->loadBattle = loadBattle;}

    void LootBattle();

private:
    std::string filepath;
    std::shared_ptr<PlayerInBattle> player;
    std::shared_ptr<BattleMenu> menu;
    std::string nextArea;
    std::unordered_map<std::string, std::shared_ptr<EnemyInBattle>> enemies;
    bool playerMove = true;
    std::string saveFilePath;
    std::string saveGameFilePath;
    bool initialStart = true;
    bool loadBattle = false;
    std::string saveSlot;

    void SavePlayerItems(nlohmann::json& saveData);

    std::string root;



};