#pragma once

#include <Engine/Scene/Scene.h>
#include <Game/Player.h>
#include <Game/Enemy.h>
#include <set>

class SaveSpot;
class TreasureChest;
class TownSpot;
class NPC;
class DialogueBox;
class RepeatPattern;

class Level : public Scene {

public:
    Level(float screenWidth, float screenHeight, std::string filepath, std::string saveFilePath, std::string saveBattleFilePath, std::string saveGameFilePath, std::string currentAreaPath, std::string root);
    //Level(std::unordered_map<std::string, std::shared_ptr<GameObject>> objects);
    ~Level();

    void Init() override;

    void LoadLevel(std::string filepath);
    void LoadPhysics(PhysicsSystem& physics) override;
    void OnEvent(const Input& input) override;
    void OnUpdate(const Input& input, PhysicsSystem& physics, float dt) override;
    void OnCollision(std::vector<CollisionEvent> collisions, float dt);

    void SaveState();
    void SaveBattleState();
    void LoadState();

    void SaveGame(std::string saveSpot);
    void LoadGame();

    void OpenChest(std::string treasureChestName);

    void RemoveEnemy(std::string enemyName);

    void UpdateCamera();

    void AddWeapon(std::string weaponName, int weaponDamage);

    static void SetSaveSlot(std::string SaveSlot){saveSlot = SaveSlot;}
    static void SetLoadGame(bool LoadGame){loadGame = LoadGame;}

    static void Reset();

private:
    std::shared_ptr<Player> player;
    void LoadGlobal(std::string globalFilePath);
    void LoadPlayer(glm::vec3 position, glm::vec3 scale, glm::vec4 color, std::string texturePath, std::string name, bool isStatic);
    void LoadTreasureChest(glm::vec3 position, glm::vec3 scale, glm::vec4 color, std::string texturePath, std::string name);
    void LoadDialogueBox(glm::vec3 position, glm::vec3 scale, glm::vec4 color, std::string texturePath, std::string name);
    std::vector<std::shared_ptr<Enemy>> enemies; 
    std::unordered_map<std::string, std::shared_ptr<SaveSpot>> saveSpots;
    std::unordered_map<std::string, std::shared_ptr<TreasureChest>> treasureChests;
    std::unordered_map<std::string, std::shared_ptr<TownSpot>> towns;
    bool gameOver = false;
    std::string filepath;
    float completionDist;
    std::string nextLevel;
    std::string saveFilePath;
    std::string saveBattleFilePath;
    std::string saveGameFilePath;
    std::string currentAreaPath;
    static bool initialStart;
    std::set<std::string> deadEnemies;
    std::string savescene;
    static bool loadGame;
    static std::string saveSlot;
    std::string areaName;
    std::unordered_map<std::string, std::shared_ptr<NPC>> npcs;
    std::shared_ptr<DialogueBox> dialogueBox;
    std::shared_ptr<RepeatPattern> repeatPattern;
    //std::map<std::string, std::shared_ptr<TreasureChest>> TreasureChests;
    std::shared_ptr<TreasureChest> treasureChestTemplate;
    std::shared_ptr<TreasureChest> CreateTreasureChest(glm::vec3 position, std::string name);
    std::shared_ptr<SaveSpot> saveSpotTemplate;
    std::shared_ptr<SaveSpot> CreateSaveSpot(glm::vec3 position, glm::vec3 scale, std::string name);

    std::string root;

    bool combatArea;

    bool moveCamera = true;
    bool enterArea = true;

    void PreviousArea();
    void SaveArea(std::string previousArea);

};